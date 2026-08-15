# star_zone firmware

Standalone replacement firmware for the Illuminate 1.0's **Top-Star and
Bot-Star** ATmega328P zone chips. **The exact same compiled image is flashed
onto both chips** — that matches the original board's own architecture (see
`docs/illuminate-hardware-notes.md` and `docs/illuminate-firmware-notes.md`:
the stock Top-Star and Bot-Star binaries are byte-identical, and any
top/bottom difference comes from wiring, not firmware). **Main and RGBW stay
on their stock, unmodified firmware.**

See the root [`README.md`](../../README.md#top-botstar-replacement-firmware)
for the compile/install walkthrough. This file covers what the firmware
actually does.

## Why this exists

The stock Top-Star/Bot-Star firmware has no Shooting Star support at all
(confirmed by decompilation — `docs/illuminate-firmware-notes.md`) and no
brightness/speed/palette controls beyond the original manual's fixed command
set. This project replaces just those two zone chips with firmware that:

- Is **wire-compatible with the existing protocol** — every command
  `components/illuminate/illuminate_protocol.h` already sends works
  unmodified, so the ESPHome component in this repo doesn't need to change.
- Adds **working Shooting Star** (`X`/`Y`/`Z` zone commands — silently
  ignored by the stock chip, fully implemented here).
- Adds a small set of enhancements from `docs/illuminate-enhanced-prd.md`,
  scoped to what makes sense on a single 16-LED zone strip: master
  brightness, per-effect speed, smooth on/off, an identify/install mode, a
  palette-driven twinkle mode, and a meteor-shower auto-fire mode for
  Shooting Star.

## Boot behavior

The strip stays fully off from power-on until the first command of any kind
is received (`StarZoneController::booted_dark_` in `controller.h`/`.cpp`).
Earlier versions rendered the default Twinkle effect immediately at boot,
which looked like an unprompted, roughly-synchronized "blinking heartbeat"
across all the LEDs before anything had actually been commanded — not useful
behavior for a controller that's supposed to reflect the last command it was
given.

## Persistence across power cycles

Since `controller.cpp`'s `load_persisted_state_()`/`save_persisted_state_()`
(EEPROM), the chip remembers its settings across power loss instead of
always coming back up in the boot-dark state above:

- **Saved**: effect, transition, color, star dimmer, master brightness,
  shooting-star brightness (`!B`), effect speed, palette, the Wipe/Fade
  transition duration (`!F`), and each zone's `!L` tuning (trail/speed
  override/decay).
- **Not saved, deliberately**: identify mode, meteor-shower active state,
  and whether a shooting-star zone is actively firing (`X`/`Y`/`Z`). These
  read as in-progress *actions* rather than steady-state *settings* —
  auto-resuming automatic firing after a power cycle without an explicit
  command seemed like the wrong default. If you want any of these
  persisted too, it's a small addition to `PersistedState` and
  `load_persisted_state_()`/`save_persisted_state_()`.
- **Power on/off (`!O`) is deliberately NOT saved — the chip always boots
  ON.** This was persisted originally and it was a mistake: hit power-off,
  power-cycle the board, and it comes back dark with no indication whether
  it is remembering a setting or has failed. Worse, while powered off,
  *nothing else lights the strip* — colors and effects still render, then
  get scaled to zero by the power multiplier — so every obvious recovery
  attempt silently does nothing. Boot-on is the recoverable default.
- **EEPROM contents are treated as untrusted input on load.** Every field
  is re-validated/clamped through the same rules its setter enforces,
  rather than being assigned raw. A stale or corrupted record could
  otherwise produce a state no command can undo — `dimmer_pct = 0` is
  permanently dark and is *not* fixable with `!O,1*` (power level and
  dimmer are independent multipliers), and an out-of-range `effect` enum
  matches no case in `effects::render()`, leaving the pixel buffer
  untouched forever. Invalid fields fall back to defaults.
- **Saves are torn-write safe.** `magic` is the last field of
  `PersistedState` and is written in a separate third phase: the existing
  magic is first zeroed, then the payload is written, then the magic is
  stamped. Losing power at any point leaves the record without its magic,
  so it is cleanly rejected at next boot instead of loading half-new,
  half-stale fields. (The first implementation wrote magic first, so an
  interrupted save left a record that validated but carried stale data.)
- **Writes are debounced** by `kPersistDebounceMs` (3000ms, `controller.h`)
  — a burst of changes (e.g. dragging through several sliders) costs one
  EEPROM write once things go quiet, not one per command. This also means
  the very last change made in the 3 seconds before a power cut may not be
  saved.
- A magic byte + version byte (`kPersistMagic`/`kPersistVersion`,
  `controller.h`) guard against misreading blank/erased EEPROM (which reads
  as `0xFF` per byte) or a stale layout from a firmware version with a
  different `PersistedState` shape — either case is treated as "nothing
  saved yet" and the chip falls back to the normal boot-dark default.
  **Bump `kPersistVersion` any time you change `PersistedState`'s fields**,
  or a chip flashed with the new firmware could misinterpret old-layout
  bytes as valid. (Currently v2; v1 persisted `power_on` and put `magic`
  first.)
- Not yet bench-tested — EEPROM read/write on a real chip, and specifically
  whether restoring shooting-star trail/speed/decay per zone actually comes
  back correctly, needs verification on real hardware before relying on it.

## What's original design vs. what's ported

- **Legacy command parsing** (`protocol.cpp`) is transcribed directly from
  the confirmed wire format in `docs/illuminate-firmware-notes.md` and
  `components/illuminate/illuminate_protocol.h`.
- **The six Starry Night effects and four color transitions**
  (`star_effects.cpp`, `controller.cpp`'s `compute_active_color_`) are
  **original implementations aimed at visual parity**, not a decompiled
  port. The Ghidra notes describe the original's per-star state layout but
  never extracted its actual animation math, and this project's own PRD
  states visual parity — not byte-exact reproduction — is the bar
  (`docs/illuminate-enhanced-prd.md`, Non-goals).
- **Shooting Star** (`shooting_star.cpp`) has no original behavior to match
  at all — it's new functionality, designed from the wire command shape
  only.

## Command grammar

Every setting below states exactly what it does, its valid range, and what
that range is based on — numbers here are pulled directly from the source
(`config.h`, `controller.h`/`.cpp`, `shooting_star.h`/`.cpp`,
`star_effects.cpp`), not approximated.

### Legacy (byte-compatible with the original protocol)

**Effect select — `T` / `F` / `B` / `R` / `P` / `J`** (single byte, no terminator)
Selects the active Starry Night animation — Twinkle / Firefly / Breeze /
Random / Rain Drops / Jamboree — effective immediately. No parameters.
Boots to Twinkle by default (irrelevant until the first command wakes the
strip — see "Boot behavior" above).

**Color transition select — `w` / `f` / `r` / `b`** (single byte, no terminator)
Selects how the current color is displayed — Wipe / Fade / Rainbow / Blink.
This is a *modifier* layered under whichever Starry Night effect is active,
not a standalone effect (see `docs/illuminate-hardware-notes.md`'s
distinction). Boots to "none" (static color, no transition) until a
transition is explicitly selected.

**Set color — `c,R,G,B,W*`**
Sets the base color Starry Night effects render with (and the color Wipe/Fade
animate toward). `R`/`G`/`B`/`W` are each **0-255**, one byte per channel —
matches the wire format's single-byte-per-field convention and the LED
hardware's 8-bit-per-channel resolution. Values sent above 255 silently
truncate (8-bit wraparound, not clamping) — stay in range.

**Star dimmer — `D,pct*`**
Overall brightness multiplier for the whole strip. `pct` is clamped to
**10-100**. The 10% floor is inherited directly from the original hardware —
transcribed from `components/illuminate/illuminate_protocol.cpp`'s
`clamp_percent()` — the original board's dimmer was never able to turn LEDs
fully off, only dim them.

**Shooting star zone — `X`\|`Y`\|`Z`,R,G,B,W,interval_s*`**
Fires one streak on zone X/Y/Z (independent, concurrent slots — see "Why this
exists" above) immediately, then automatically fires another every
`interval_s` seconds, indefinitely, until turned off. `R`/`G`/`B`/`W` are
0-255 as above. `interval_s` is **0-65535** (a 16-bit field, per explicit
design decision after bench testing) — `0` turns the zone off (matches
`components/illuminate/illuminate_protocol.h`'s
`shooting_star_off_command`'s `{Zone},0,0,0,0,0*` shorthand, still valid),
any other value is how many seconds between shots; there's no product reason
to cap the upper end below what a 16-bit field allows (~18.2 hours).
**This diverges from `illuminate_protocol.h`'s `ShootingStarDuration`**,
which sends `0/1/2/5/10` meaning *minutes of continuous firing* — real
bench-test feedback was that "run continuously for N minutes" wasn't useful
compared to "fire predictably every N seconds," so this field's *meaning*
changed even though its wire position didn't. If you're driving this from
the existing ESPHome `shooting_star` light entity, its duration picker will
now mean seconds, not minutes.

### New enhancement commands (leading `!`)

Namespaced under a leading `!` so they can never collide with the legacy
grammar above, which only ever starts a command with a bare letter — see
`docs/illuminate-enhanced-prd.md`'s "Protocol versioning / extensibility
strategy."

| Command | Meaning |
|---|---|
| `!M,pct*` | Master brightness |
| `!B,pct*` | Shooting-star brightness, independent of the `D` star dimmer |
| `!S,speed*` | Shared effect/shooting-star speed |
| `!O,{0\|1}[,transition_ms]*` | Power on/off with a smooth fade |
| `!I,{0\|1}*` | Identify/install mode on/off |
| `!N*` / `!V*` | Identify mode: step to next/previous LED |
| `!C,palette_id*` | Palette-driven twinkle |
| `!W,{0\|1}[,avg_interval_s]*` | Meteor-shower auto mode on/off |
| `!L,zone,trail[,speed[,decay]]*` | Per-zone shooting star trail length, speed override, and decay-curve exponent |
| `!F,ms*` | Wipe/Fade color-transition duration |

**`!M,pct*` — master brightness.** Multiplies on top of the star dimmer
(`D`) as a second, independent scaling factor — `docs/illuminate-enhanced-
prd.md`'s "global scaling applied across all outputs, independent of the
existing per-zone dimmers." `pct` clamps to **0-100** (unlike the star
dimmer, 0 is allowed here — this is the control meant to actually turn the
strip fully dark without losing the dimmer/color state underneath it).

**`!B,pct*` — shooting-star brightness.** Scales shooting-star streaks
(X/Y/Z zones *and* the meteor shower) **independently of the `D` star
dimmer**, 0-100, default 100. This exists so the Starry Night layer can sit
dim as an ambient starfield while streaks cross it at full brightness —
impossible before, because `D` was applied to the composited frame and
dimmed both together.

**This changes what `D` affects.** `D` now scales the Starry Night layer
only, and is applied *before* streaks are blended in; the composited result
is then scaled by `!M` (master) and the `!O` power level. The full pipeline
is documented in `controller.cpp`'s `render_()`. There's no backward-
compatibility concern: stock hardware has no Shooting Star support at all,
so no existing setup can depend on `D` dimming streaks.

**`!S,speed*` — shared effect/shooting-star speed.** One `speed` byte
(**0-255**) drives the pacing of whichever Starry Night effect is active
*and* any shooting-star zone that hasn't been given its own override via
`!L` (see below). 0-255 was chosen because it's a single wire byte, matching
every other "amount"-style field in this protocol (R/G/B/W, brightness). It
is **not** a linear millisecond value — each effect maps the same byte to
its own period range, tuned separately so every effect still looks
reasonable across the same slider:

| Effect / transition | Period at speed=0 (slowest) | Period at speed=255 (fastest) |
|---|---|---|
| Twinkle (per-pixel flicker period) | 4000ms | 500ms |
| Firefly (per-pixel glow period) | 9000ms | 1500ms |
| Breeze (brightness-wave cycle) | 11000ms | 1200ms |
| Random (per-pixel color-change interval) | 4000ms | 300ms |
| Rain Drops (drop-trigger interval; decay itself is a fixed 260ms) | 2600ms | 250ms |
| Jamboree (hue-rotation cycle) | 5500ms | 600ms |
| Jamboree (sparkle-overlay period) | 1600ms | 250ms |
| Rainbow transition (full hue cycle) | 14000ms | 800ms |
| Blink transition (on+off cycle) | 3000ms | 200ms |
| Shooting star (time to cross the whole strip) | 3200ms | 350ms |

These "slowest" bounds were widened (previously roughly half these values)
after bench-test feedback that `speed=1` still felt too fast — if the low
end still isn't slow enough for a given effect, its bound is a single
`map(speed, 0, 255, X, Y)` call in `star_effects.cpp` (or the `RAINBOW`/
`BLINK` cases in `controller.cpp`'s `compute_active_color_`, or
`shooting_star.cpp`'s `streak_ms`) — raise `X`.

**`!O,{0|1}[,transition_ms]*` — power on/off.** `1`/`0` fades the strip in
or out smoothly rather than snapping. `transition_ms` (default **500**ms if
omitted) has no upper clamp currently — any `uint16_t` value (up to 65535ms
≈ 65s) is accepted as-is.

**`!I,{0|1}*` / `!N*` / `!V*` — identify/install mode.** `!I,1*` lights
exactly one LED (index 0) and turns off all effect rendering, for physically
tracing which LED is which during installation; `!N*`/`!V*` step to the
next/previous LED, wrapping across the full strip (**0 to `kLedCount`-1**,
i.e. 0-15 on a 16-LED strip); `!I,0*` resumes normal rendering.

**`!C,palette_id*` — palette-driven twinkle.** Twinkle/Firefly/Breeze/Rain
Drops/Jamboree normally render using the last `c,R,G,B,W*` color; `palette_id`
**1-5** switches that source to a built-in palette instead
(`palette.cpp`: 1=Fire, 2=Ocean, 3=Forest, 4=Candy — arbitrary "themed" sets,
no hardware reference to match). `0` disables the palette and reverts to the
plain commanded color. Values above 5 are treated as `0` rather than
erroring.

**`5` = Rainbow Cycle** is a special case rather than a 4-stop gradient:
every LED shows the **same hue at the same instant**, and the whole strip
fades smoothly through the full spectrum together, paced by the `!S` effect
speed — **60s** for a complete sweep at speed 0, **2s** at speed 255.

Do not confuse it with the `r` **RAINBOW color transition**, which is a
different look built from the same math: `r` deliberately adds a per-pixel
hue offset so the spectrum is spread *across* the strip (a rainbow you can
see all of at once, drifting), whereas Rainbow Cycle omits that offset so
the strip is a single, uniformly-changing color. The per-pixel offset is
the entire difference — see `source_color()` in `star_effects.cpp` versus
the `RAINBOW` case in `controller.cpp`'s `compute_active_color_()`.

Note that the selected Starry Night effect still applies its own per-pixel
*brightness* animation on top, so with Twinkle the LEDs share a hue while
twinkling independently. For a completely flat, uniform strip there is no
"solid" effect to select — that would need a new effect, not a palette.

**`!W,{0|1}[,avg_interval_s]*` — meteor-shower auto mode.** An *additional*,
automatic streak generator layered on top of everything else — it does not
use the X/Y/Z zones (it has its own internal slot) and needs no color of its
own: each shot uses the active palette (if `!C` is set) or the current
Starry Night color otherwise. Unlike the X/Y/Z zones' fixed-cadence
`interval_s`, this mode's timing is **randomly jittered** around
`avg_interval_s` (default **20**, minimum **1** — `0` is treated as `1`
rather than a divide-by-zero, no upper clamp) so it reads as organic
background sparkle rather than a predictable per-zone shot. This is the
actual distinction from firing an X/Y/Z zone with a short `interval_s`:
meteor shower is randomized timing + ambient color, zones are exact timing +
an explicitly chosen color.

**`!L,zone,trail[,speed[,decay]]*` — per-zone shooting star tuning.** `zone`
is `X`/`Y`/`Z`. `trail` (fade length behind the streak's head, in pixels) is
clamped to **1-40** — 1 is the minimum to avoid a divide-by-zero in the fade
math (a trail of exactly 0 pixels is undefined), 40 is a generous ceiling
(2.5x the 16-pixel strip) so a trail can span or exceed the whole strip.
`speed` (optional) overrides the shared `!S` speed for this zone only,
**0-255**; omit it, or send **256 or higher**, to clear the override and
fall back to `!S`. `decay` (optional, only reachable if `speed` is also
present — send 256+ there to keep the shared speed while still setting
decay) is a fade-curve exponent, wire value **10-500** meaning **0.10-5.00**
(divide by 100); it's applied as `brightness = 255 * linear_ramp ^ decay`,
where `linear_ramp` is the same head-to-tail 1→0 ramp `trail` always
produced. `decay = 100` (exponent 1.00) is pure linear — this firmware's
original hardcoded shape, where `trail` was the *only* knob and its falloff
steepness was always exactly `255/trail`. Values above 100 dim faster near
the head (a shorter-*looking* glow without shortening `trail` itself);
below 100 stays brighter for longer before dropping off near the very end.
Omitting `decay` resets it to 100, same as omitting `speed` resets to no
override — `!L` always fully re-specifies a zone's tuning, it doesn't patch
individual fields. Defaults (trail=3px, no speed override, decay=linear)
reproduce this firmware's original hardcoded behavior, so a zone that's
never received `!L` behaves exactly as before this command existed.

**`!F,ms*` — Wipe/Fade transition duration.** How long the `w`/`f` color
transitions take to animate from the old color to the new one. Clamped to
**50-10000**ms — 50ms floors it just above "instant" (a lower value would
defeat the point of having a transition at all), 10000ms (10s) is a
practical ceiling so a typo doesn't produce an effectively-frozen
transition. Default is **700**ms (this firmware's original hardcoded value).

The original firmware's undocumented `O` single-byte command
(`docs/illuminate-firmware-notes.md`) is intentionally left unhandled —
its behavior was never confirmed on real hardware, so there's nothing to
replicate, and it doesn't collide with anything above (the `!O` power
command is a different, two-or-more-byte sequence).

## Open questions before you trust this on real hardware

These are called out inline in the source (`config.h` mainly) but worth
repeating:

1. **LED channel count and wire byte order.** `config.h`'s `kLedColorOrder`
   defaults to `NEO_GRBW` (4-channel SK6812-style, chosen because the
   protocol carries 4-channel RGBW colors). `docs/illuminate-enhanced-prd.md`
   lists byte order as an explicitly open question, and a quick check
   against `captures/starry_colors/*.json`+`.csv` (sent command vs. decoded
   pixel) doesn't cleanly resolve it. **Send a known solid color
   (`c,255,0,0,0*`) and visually confirm before trusting any other color
   this firmware produces.**
2. **Shooting Star behavior is a new design, not a clone** — there's no
   original SS-firmware capture to match against (PRD open question #5).
   Trail length and speed are tunable per zone at runtime via `!L` (see the
   command table above) rather than requiring a firmware rebuild - defaults
   (3px trail, shared `!S` speed) reproduce the original hardcoded values.
3. Effect timings in `star_effects.cpp` are similarly original — adjust
   freely, nothing here is "the correct" Twinkle/Firefly/etc. speed.
4. **SOLVED — "Top-Star reception of longer commands" was never a hardware
   problem.** For a long time this was recorded here as an unexplained
   quirk: single-byte commands (effect select) worked, while multi-byte
   comma-delimited ones (`c,R,G,B,W*`) silently did nothing, and it was
   assumed to be marginal UART signal integrity on one chip's reception
   path, possibly related to `docs/illuminate-hardware-notes.md`'s open
   question about whether Top-Star's RXD shares the broadcast bus.

   The real cause was in this firmware: `main.cpp`'s `loop()` called
   `controller.tick()` (and therefore `Adafruit_NeoPixel::show()`) on every
   single iteration. `show()` disables interrupts for the whole pixel
   transmission — ~640µs for 16 RGBW pixels — so interrupts were off well
   over half the time and the UART RX interrupt couldn't run. Bytes were
   dropped. A single-byte command only has to survive one blackout window;
   a 12-byte command has to survive twelve, which is exactly why short
   commands "worked" and long ones didn't. **Expanding the strip from 8 to
   16 LEDs doubled the blackout and took it from flaky to completely
   non-functional** — no command of any length got through.

   Fixed by rendering on a fixed ~50 FPS timer (`kRenderIntervalMs` in
   `main.cpp`) while still polling serial every iteration, dropping the
   interrupts-off duty cycle to ~3%.

   Diagnosed on real hardware by splitting the two paths apart with
   standalone test firmware: one build that lit the strip unconditionally
   at boot (proving LED output, power, and the data line were fine), and
   one that displayed each received serial byte in binary on the LEDs
   (proving bytes arrived intact at 9600 baud). With both halves proven
   good in isolation but the real firmware still unresponsive, the fault
   had to be the interaction between them. **If a symptom like this recurs,
   bisect output vs. input with dedicated test firmware before theorizing
   about the code** — it is far faster than reasoning from source, and both
   test builds took under a minute to write and flash.
5. **Fixed: the resync logic from item 4 was itself breaking `!`-prefixed
   commands.** The mitigation described above — abandon a buffered command
   and start fresh as soon as a byte arrives that looks like a new command's
   start — didn't account for `!`-prefixed commands legitimately containing
   letters as field values (e.g. `!L,X,8,200*`'s `X` zone field). Since `X`
   is also a legacy command-start byte, the old resync logic fired on it
   mid-`!L`-command, truncating the buffer and silently dropping the whole
   command (confirmed via real hardware: `!L` trail-length changes were
   having no effect at all). Fixed by scoping the resync check to only
   legacy (non-`!`) commands, where it's actually needed and where no field
   is ever a letter — see the comment above `poll()`'s resync check in
   `protocol.cpp`.
6. **EEPROM persistence (see "Persistence across power cycles" above) is
   unverified on real hardware.** The load/save logic and the ATmega328P's
   EEPROM are both standard/well-understood, but nobody has yet power-cycled
   a flashed chip and confirmed settings actually come back correctly —
   treat it as implemented-but-unconfirmed until that's done.

## Building and flashing

See the root README's
[Top/Bot-Star replacement firmware](../../README.md#top-botstar-replacement-firmware)
section.
