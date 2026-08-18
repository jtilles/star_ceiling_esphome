## firmware/star_zone/ — standalone Top-Star/Bot-Star replacement firmware

A PlatformIO AVR project that replaces the stock Top-Star/Bot-Star
ATmega328P zone chips' firmware entirely (ISP-flashed, same image on both
chips — `platformio.ini` targets `pro16MHzatmega328`/`atmelavr`). Main and
RGBW stay on stock firmware; only the two zone chips get this. Wire-
compatible with `components/illuminate/illuminate_protocol.h`'s legacy
grammar, plus a `!`-prefixed enhancement command set of its own — see
`firmware/star_zone/README.md` for the full command reference (every
setting's exact valid range and what that range is based on) and the root
`README.md`'s "Top/Bot-Star replacement firmware" section for compile/
install steps. This has been bench-flashed and iterated against real
hardware across multiple rounds of feedback, not just compiled.

**Architecture**: `config.h` is the single source of truth for pins/LED
count/etc — every array and loop elsewhere reads `kLedCount` rather than
hardcoding it, so e.g. going from 8→16 LEDs was a one-line change.
`controller.h`/`.cpp` (`StarZoneController`) owns all runtime state and the
render pipeline; `protocol.cpp` parses serial bytes and calls straight into
it; `star_effects.cpp`/`shooting_star.cpp`/`palette.cpp` are the rendering
logic `controller.cpp` calls into.

**Gotchas hit while building/iterating this (read before touching
protocol.cpp or the ISP flashing flow again)**:

- **The serial parser's "resync on stray command-start byte" logic
  (`protocol.cpp`'s `poll()`) must never fire while inside a `!`-prefixed
  command.** It exists to recover from a legacy command that lost its
  terminator (e.g. a dropped UART byte), but `!`-prefixed enhancement
  commands can legitimately contain letters as field values — e.g.
  `!L,X,8,200*`'s `X` zone field — and those letters (`X`/`Y`/`Z`/`c`/`D`)
  are *also* legacy command-start bytes. This actually shipped once: the
  resync fired mid-`!L`-command and silently dropped it, confirmed only via
  real-hardware testing ("trail length isn't doing anything"). Fixed by
  scoping the check to `buf[0] != '!'`. Any future change to the resync
  logic or the enhancement grammar needs to re-verify this doesn't regress.
- **`avrdude`/USBasp ISP uploads intermittently fail verification at a
  fixed byte offset** (`avrdude: verification error, first mismatch at byte
  0x0042` was the recurring one) even though the write step reports
  success — this happened repeatedly across sessions, on a chip that had
  flashed fine minutes earlier. Slowing the ISP clock (`platformio.ini`'s
  `-B10` upload flag) did **not** fix it. The actual fix: explicitly erase
  the chip first, then upload — `pio run --target upload`'s implicit
  auto-erase isn't reliably completing on this chip/programmer combo.
  `avrdude` isn't on `PATH`; PlatformIO's bundled copy is at
  `~/.platformio/packages/tool-avrdude/avrdude.exe` (pass `-C` pointing at
  the sibling `avrdude.conf`). Full working command:
  `avrdude.exe -C avrdude.conf -c usbasp -p m328p -e`, then retry the
  normal `pio run --target upload`.
- **Shooting star's trailing numeric field means something different here
  than in `illuminate_protocol.h`.** The ESPHome component's
  `ShootingStarDuration` sends `0/1/2/5/10` meaning *minutes of continuous
  firing*; `star_zone` firmware reinterprets the same wire position as
  `interval_seconds` (0-65535) — fire one streak, then another every N
  seconds, indefinitely, until 0 turns it off. Changed deliberately based on
  real bench-test feedback ("it runs continuously instead of firing
  periodically"). **`components/illuminate/` has since been updated to
  follow this firmware rather than the stock board** — the old
  `ShootingStarDuration` enum is gone, replaced by a 16-bit
  `interval_seconds`, and the light platform's `duration:` option is now
  `interval:` (any time period, 1-65535s). The component also gained the
  whole `!` enhancement command set as public `IlluminateHub::send_*`
  methods, surfaced as template entities in `cooper_ceiling.yaml` rather
  than as new codegen platforms. If you change a command's grammar or a
  field's range here, that component and `tests/test_illuminate_protocol.cpp`
  now need updating in lockstep — this firmware's README is the authority on
  every range, and the component's clamps are transcribed from it.
- **`RGBW` (`types.h`) needs an explicit constructor**, not pure aggregate
  init — this toolchain's default C++ standard (`gnu++11`) doesn't treat a
  struct with default member initializers as an aggregate, so
  `RGBW{1,2,3,4}` fails to compile without one.
- **Random and Jamboree ignore the commanded color entirely** (Random always
  generates its own per-pixel hue; Jamboree does too unless a palette is
  active via `!C`) — setting color to black does **not** turn either of them
  off, it just has no visible effect. This caused real user confusion
  ("why doesn't black turn it off?"). The actual off switch is `!O,0[,ms]*`
  (power off) — it's a post-render output multiplier applied after every
  effect in `render_()`, so nothing can bypass it, unlike the color field.
  The HTML console has a dedicated "⏻ Power Off" button in the header for
  exactly this reason.
- **EEPROM persistence (`controller.cpp`'s `load_persisted_state_`/
  `save_persisted_state_`) is unverified on real hardware** — implemented
  but not yet bench-tested (see `firmware/star_zone/README.md`'s
  "Persistence across power cycles" and open question #6). If settings
  don't survive a power cycle as expected, check this first before assuming
  it's something else.
- **Pseudo-random helpers need real avalanche mixing.** `color_utils::
  hash8()`'s first implementation multiplied inputs by fragments of the
  golden-ratio constant and shifted — looked fine at a glance, but its
  second parameter (a slowly-incrementing "time bucket," used by the
  RAIN_DROPS/RANDOM effects) barely moved the output per `+1`, so the same
  handful of pixels stayed lit for many consecutive ticks instead of a
  fresh draw each interval ("rain drops are not very random... like the
  same lights just flashing" — a real bench-test report, not something
  caught by just reading the code). Replaced with Thomas Wang's 32-bit
  integer hash. If more hash-like helpers get added, verify avalanche
  empirically (e.g. a Python port checking consecutive-bucket deltas), not
  by inspection.
