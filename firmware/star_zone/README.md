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

**Legacy (byte-compatible):**

| Command | Meaning |
|---|---|
| `T`/`F`/`B`/`R`/`P`/`J` | Starry effect: Twinkle/Firefly/Breeze/Random/Rain Drops/Jamboree |
| `w`/`f`/`r`/`b` | Color transition: Wipe/Fade/Rainbow/Blink |
| `c,R,G,B,W*` | Set starry color |
| `D,pct*` | Star dimmer (10-100) |
| `X`\|`Y`\|`Z`,R,G,B,W,minutes*` | Shooting star zone (`minutes`: 0=off,1,2,5,10) |

**New, namespaced under a leading `!` so it can never collide with the
grammar above** (see `docs/illuminate-enhanced-prd.md`'s protocol
versioning strategy):

| Command | Meaning |
|---|---|
| `!M,pct*` | Master brightness (0-100), multiplies on top of the star dimmer |
| `!S,speed*` | Effect/shooting-star speed (0-255) |
| `!O,{0\|1}[,transition_ms]*` | Power on/off with a smooth fade (default 500ms) |
| `!I,{0\|1}*` | Identify/install mode on/off |
| `!N*` / `!V*` | Identify mode: step to next/previous LED |
| `!C,palette_id*` | Palette-driven twinkle (0 = off, use the last `c` color; 1-4 = built-in palettes) |
| `!W,{0\|1}[,avg_interval_s]*` | Meteor-shower auto mode on/off (default 20s average interval) |

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
   Tune `shooting_star.cpp`'s streak speed/trail length to taste.
3. Effect timings in `star_effects.cpp` are similarly original — adjust
   freely, nothing here is "the correct" Twinkle/Firefly/etc. speed.
4. **Top-Star reception of longer commands.** First-hardware-test finding:
   on one Top-Star chip (flashed and verified via ISP, receiving and acting
   on single-byte commands like effect-select fine), multi-byte comma-
   delimited commands (`c,R,G,B,W*` in particular) were silently having no
   effect, while the same commands worked on Bot-Star. Single-byte commands
   surviving while longer ones fail points at marginal UART signal
   integrity on that chip's specific reception path corrupting/dropping a
   byte mid-command, rather than a logic bug (the parser is identical for
   both chips). This is also the exact hardware question flagged as
   unresolved in `docs/illuminate-hardware-notes.md`: "Confirm whether
   Top-Star's hardware RXD is on the same broadcast bus as Main/Bot-Star."
   `protocol.cpp`'s parser now resyncs itself (abandons a buffered command
   that never saw its terminator as soon as the next command-start byte
   arrives) so one corrupted command can no longer also eat the next
   legitimate one, but that's a mitigation, not a fix for a wiring/signal
   integrity problem — if commands are still unreliable on a given chip
   after this, capture that chip's RXD line during a `c,...*` send (the
   Saleae tooling in `docs/tooling/` already does exactly this) to see
   whether bytes are actually arriving intact.

## Building and flashing

See the root README's
[Top/Bot-Star replacement firmware](../../README.md#top-botstar-replacement-firmware)
section.
