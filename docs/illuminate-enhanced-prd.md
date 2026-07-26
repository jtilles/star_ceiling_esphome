# PRD: Illuminate Enhanced — Consolidated Single-MCU Firmware & Schematic Revision

## Status
Draft — pre-prototype. Intended as both an internal project spec and a pitch document for
Fusion Synergy (original designer: Eran).

## Summary

The stock Illuminate 1.0 controller uses four ATmega328P MCUs (Main + Top-Star + Bot-Star + RGBW)
connected via bespoke bit-banged serial links, plus a socketed wireless module (HM-10 BLE or
ESP8266) and a MAX3232 for wired RS232. This is unnecessary complexity: one ATmega328P has enough
free GPIO and PWM channels to directly drive all outputs (two WS2812 zones, ambient RGBW PWM, and
a shooting-star WS2812 daisy-chain) without any inter-chip relaying. This project delivers a
revised firmware image and schematic for a single-MCU version of the board, paired with a
WiFi-to-serial ESP module (ESPHome-based) that is protocol-compatible with the existing product
line.

## Goals

1. **Reduce BOM and assembly complexity** — one ATmega328P instead of four, eliminating three
   ISP programming steps, three crystals, and all inter-chip wiring/link circuitry.
2. **Reduce latency and failure surface** — remove the serial-relay hop between Main and the zone
   MCUs entirely.
3. **Full backward compatibility** — the enhanced firmware must accept every command in the
   original RS232 protocol unchanged, so it functions as a drop-in replacement for existing
   customers/integrations (including the official app, if it still uses the same command set over
   BLE/wired serial) with zero required changes on their end.
4. **New capabilities are additive, not breaking** — new functionality is exposed via new command
   syntax that cannot collide with the original protocol's grammar, so old and new controllers can
   coexist in the field and existing third-party integrations keep working untouched.
5. **Deliverable suitable to hand to Fusion Synergy** as a concrete "here's a simpler way to build
   this" proposal — schematic revision + firmware source + rationale.

## Non-goals

- Not attempting to reverse-engineer or reproduce the original firmware's exact internal
  algorithms byte-for-byte (see hardware notes doc's "reimplementation strategy" section) — visual
  parity of effects is the bar, not binary equivalence.
- Not in scope: redesigning the physical LED fixtures themselves (fiber runs, diffusers, shooting
  star mechanical assembly if any exists on SS units).
- Not in scope for this project specifically: the "native/off-the-shelf" ESPHome component (that's
  a separate, already-scoped project targeting the *existing* 4-chip board's original firmware
  as-is).

## Background

See `illuminate-hardware-notes.md` for the full reverse-engineering trace this PRD builds on,
including the confirmed WS2812 LED protocol, the confirmed SP-vs-SS firmware split on original
units, and the pin trace that identified which Main GPIOs are free once inter-chip links are
removed. See `illuminate-enhanced-pinmap.md` for the specific pin reassignment.

## Architecture

```
[Home Assistant] <--WiFi/API--> [ESPHome ESP8266/ESP32] <--UART, 9600 8N1--> [ATmega328P "Main"]
                                  (thin WiFi-to-serial                        |-- WS2812: Star array
                                   bridge, minimal logic)                     |    (16 LEDs, Top+Bottom
                                                                               |     wired in series —
                                                                               |     Shooting Star is a
                                                                               |     firmware mode on
                                                                               |     this same array,
                                                                               |     not separate
                                                                               |     hardware)
                                                                               |-- PWM x4: Ambient RGBW
                                                                               |-- Wired RS232 (MAX3232,
                                                                                    unchanged, for legacy
                                                                                    wired installs)
```

The ESP's role stays intentionally thin — it is a WiFi-to-serial converter exposing ESPHome
entities, translating HA actions into the same command grammar the Main MCU already/will
understand, per the existing ESPHome component project. All effect logic, LED driving, and timing
lives on the Main MCU, preserving the graceful-degradation property (ceiling continues running its
last commanded state if the ESP reboots/loses WiFi) that was part of the original architecture's
intent even if not well executed.

## Functional requirements

### Backward-compatible command set (must work unmodified)
- All Starry Night effects: Twinkle, Firefly, Breeze, Random, Rain Drops, Jamboree (`T/F/B/R/P/J`)
- All Starry Night color transitions: Wipe, Fade, Rainbow, Blink (`w/f/r/b`)
- Starry Night colors: `c,R,G,B,W*`
- Star dimmer: `D,{pct}*` (10-100)
- Shooting Star: `{X/Y/Z},R,G,B,W,{time}*` — **now functional on every unit**, since this
  revision has no SP/SS firmware split. `X`/`Y`/`Z` (SS1/SS2/SS3) map to **virtual zones** — defined
  sub-ranges of the single 16-pixel Star array — rather than physically separate outputs, since
  Shooting Star is a firmware effect mode on the same array as Starry Night, not dedicated hardware.
  Exact zone-to-pixel-range mapping is an implementation decision, not yet finalized.
- Ambient effects: `e,{speed}` (Fade) / `k,{speed}` (Blink) / `n` (No Effect)
- Ambient colors: `A,R,G,B,W*`
- Strip dimmer: `d,{pct}*` (10-100)

### New capabilities (additive command namespace, non-colliding with above)
- **Master brightness control** — independent of the existing per-zone dimmers, a global scaling
  applied across all outputs.
- **Per-effect speed control** — adjustable rate for Twinkle/Firefly/Breeze/etc. and for the
  shooting star "meteor" animation, exposed as a continuous parameter rather than the original's
  fixed/implicit speed.
- **Shooting Star: full parametric control** — color, speed, trail length/fade amount, and
  simultaneous multi-streak support (multiple concurrent shooting stars with independent
  color/timing, vs. original's single-shot per zone).
- **Shooting Star: "meteor shower" auto mode** — randomized automatic firing (interval, color
  variance, position) as a distinct mode from manually-triggered single shots.
- **Identify / install mode** — steps through LEDs one at a time (or N at a time) with
  adjustable dwell time and manual next/previous stepping, for physical installation and
  post-install service. Applies to all three WS2812 outputs (Top, Bottom, Shooting Star module).
- **Palette-driven twinkle** — twinkle/sparkle effects can use a themed color palette instead of a
  single fixed color.
- **Noise-based ambient effect(s)** — slow organic color drift (Perlin-noise-driven), new effect
  family not present in the original.
- **Smooth on/off transitions** — fade in/out over a configurable duration on power toggle, rather
  than an instant snap.
- **Raw passthrough / escape hatch** — the ESPHome hub component should expose a raw
  command-send action for anything not yet modeled as a first-class entity, so new firmware
  commands can be exercised immediately without waiting on component updates.

### Hardware requirements
- Single ATmega328P per pin map in `illuminate-enhanced-pinmap.md`.
- Ambient RGBW driven via true hardware PWM (4 channels), not bit-banged.
- Top and Bottom 8-LED arrays wired in series into one 16-LED string, driven from a single WS2812
  data pin. No separate Shooting Star hardware — it's a firmware mode on this same array.
- Wired RS232 (MAX3232) path and wireless UART-to-ESP path preserved exactly as in the original
  design, per backward compatibility requirement.
- Physical LED wiring order is known/controlled (installer wires the array in physical sequence),
  so no pixel-order remapping table is needed for Shooting Star's chase animation across the
  combined array.

## Open technical questions (must resolve before finalizing schematic)

1. **Pin 12 / Pin 31 MAX232 conflict** — continuity check needed to determine whether the wired
   RS232 TX source is hardware USART or bit-banged software serial via the under-ESP jumper. This
   determines whether the new schematic needs to preserve that jumper's function or can simplify
   it.
2. **Pin 25 (PC2) status** — currently untraced/unresolved; confirm free via continuity before
   allocating to any new function, even as a spare.
3. **RGB(W) byte order and bit depth for WS2812 output** — confirm from existing logic analyzer
   captures (protocol itself confirmed as WS2812; exact color channel ordering not yet
   documented here).
4. **Ambient PWM frequency/resolution** — need to pick a PWM frequency high enough to avoid
   visible flicker especially when dimmed low, balanced against Timer0/Timer1 configuration
   already used elsewhere on the chip. Not yet specified.
5. **Shooting Star effect behavior is not yet characterized** — the bench unit used for the
   original RS232/logic-analyzer work runs SP (static) firmware and does not respond to Shooting
   Star commands. A unit running SS firmware still needs to be obtained/flashed and captured
   (logic analyzer on the LED data line during real Shooting Star commands) before the effect can
   be faithfully reimplemented — currently only Starry Night effects/colors have been captured
   against real hardware.

## Protocol versioning / extensibility strategy

New commands must use a prefix or character range that cannot appear in the original protocol's
grammar, so a single firmware image can unambiguously distinguish legacy commands from new ones.
(Original protocol commands are: single uppercase/lowercase letters, and comma-delimited strings
starting with `c`, `A`, `D`, `d`, `X`, `Y`, `Z`, `e`, `k`, `n`, terminated by `*` where applicable
— exact reserved character/prefix space for new commands to be finalized during implementation,
but should be decided and documented before writing firmware, not discovered ad hoc.)

## Rollout plan

1. Build and bench-test a single-MCU prototype against the pin map, validating all backward-compat
   commands produce visually identical results to an original 4-chip unit.
2. Wire the combined 16-LED Star array (Top+Bottom in series), validate the physical wiring order
   assumption holds and both Starry Night and Shooting Star chase animations look correct across
   the full array without remapping. Requires capturing real Shooting Star behavior first (see
   open question above) — either from an SS-firmware unit or by having Eran/Fusion Synergy provide
   reference behavior/timing.
3. Implement and validate new capabilities (master brightness, speed control, identify mode,
   meteor shower, palette twinkle, noise ambient).
4. Update/extend the existing ESPHome component to expose new capabilities as HA entities,
   confirm the raw-passthrough escape hatch works for anything not yet modeled.
5. Document schematic changes (BOM reduction, pin map, removed components: 3x ATmega328P, 3x
   crystal, associated passives) as a clear before/after for the pitch to Fusion Synergy.
6. Package firmware source, schematic revision, and rationale doc for handoff/publication.

## Risks

- **Single point of failure** — consolidating to one MCU removes the "zone MCUs keep running if
  Main crashes" property the original architecture nominally had (even if it wasn't well
  exploited). Acceptable given the reliability/simplicity tradeoff was made deliberately (see
  prior discussion), but worth stating explicitly as a known tradeoff in the pitch to Fusion
  Synergy rather than leaving it implicit.
- **PWM/WS2812 timing interaction on one chip** — driving 3 WS2812 outputs and 4 PWM channels from
  a single 16MHz AVR while also servicing UART needs bench validation for any visible artifacts
  (flicker, timing glitches) under worst-case simultaneous load (e.g. all three WS2812 zones
  animating plus ambient fading at once).
- **Unresolved pin questions (12/31 conflict, pin 25)** block finalizing the schematic and should
  be closed out before committing to a board revision.
