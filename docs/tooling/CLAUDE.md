## docs/tooling/ — Saleae capture-sweep & LED protocol investigation

Goal: figure out the byte-level protocol the P-TOP/P-BOTTOM zone MCUs use to
drive the physical star LEDs, by sending every known RS232 command while a
Saleae logic analyzer captures the LED data lines.

- `config.py` — hardware-specific settings (serial port, Saleae device ID,
  channel numbers, sample rate). Already filled in with real working values
  for the current bench setup — don't reset these to TODO/None.
- `commands.py` — builds the full ~59-command RS232 sweep list from the
  protocol tables.
- `capture_sweep.py` — runs the full sweep, decodes each capture via
  Saleae's built-in analyzers, exports one CSV + JSON sidecar per command
  under `captures/`.
- `list_saleae_devices.py` — one-off helper to find `SALEAE_DEVICE_ID`.
- `diagnose_ws2812_timing.py`, `analyze_raw_timing.py`,
  `decode_ws2812_rgbw.py`, `raw_csv_utils.py` — raw bit-level diagnostic
  tools, built after discovering the built-in decoder was wrong (see next
  section). Not part of the main sweep; used for one-off manual investigation.

### CRITICAL FINDING: the physical star LEDs are RGBW, not RGB

Saleae's built-in **"Addressable LEDs (Async)"** analyzer (used by
`capture_sweep.py`'s decoded export) only supports RGB chips (24 bits/LED —
WS2811/WS2812B/WS2813/TM1809/TM1804/UCS1903/LPD1886). It has **no RGBW
mode** (confirmed against Saleae's own analyzer source and a support-forum
thread confirming this limitation is permanent, not a config option).

The real hardware is RGBW (32 bits/LED), confirmed by raw bit-level capture:
sending `c,255,0,0,0*` (solid Red) produces a rock-steady, real-hardware
result of **256 bits per LED-refresh frame**, which is exactly **8 LEDs ×
32 bits** (24-bit RGB framing gives a non-integer 10.67 LEDs — the tell).
This also means each row has **8 physical LEDs, not 11** — the "11 pixels"
figure from the decoded (RGB-assuming) sweep data was itself an artifact of
the wrong framing and should be disregarded.

**Practical consequence:** all decoded CSVs produced by the main
`capture_sweep.py` run (the `captures/` directory checked into/left in the
repo from before this was discovered) are built on the wrong 24-bit RGB
framing and are **not reliable** for per-pixel color analysis — solid-color
commands decode as a rotating 4-hue garbage pattern instead of a uniform
color. They're still useful for the UART command bytes (that decode is
correct) and rough effect-vs-effect statistical comparison, but not for
literal per-pixel RGBW values.

**Fix in progress:** `decode_ws2812_rgbw.py` decodes raw digital captures
assuming 32-bit RGBW frames instead. Byte order within each 32-bit LED
frame is confirmed so far: **`byte1` = Red**. Green/Blue/White byte
positions are still unconfirmed (in progress as of this writing) — next
step was sending isolated named-palette colors (Green `c,0,255,0,0*`, then
Blue, then White `c,0,0,0,255*`) one at a time via `diagnose_ws2812_timing.py`
and decoding each with `decode_ws2812_rgbw.py` to see which byte position
lights up for each channel.

**Open question, needs hardware-side confirmation:** a custom/arbitrary
RGBW value (`c,100,150,200,50*`, not a factory palette entry) did NOT
appear to change the physical output when tested — the capture kept
showing the previous Red state (with some drift, possibly leftover effect
animation) rather than the new values. This suggests the firmware may only
honor palette-exact RGBW values despite the manual documenting arbitrary
0-255 values as the format. Needs a "did the ceiling visually change?"
confirmation and/or more testing before concluding either way — don't
assume arbitrary RGBW works when building the ESPHome light platform's
color picker without validating this further.

### Known gotchas with the Saleae Automation API (`logic2-automation`)

- **Directory/filepath arguments are resolved by the Logic 2 desktop
  process, not the calling Python script.** A relative path gets
  interpreted against Logic 2's own working directory (often unwritable,
  e.g. under Program Files) → "Access is denied." Always resolve to an
  absolute path before passing to `export_raw_data_csv` / `export_data_table`.
- `add_analyzer()` requires the analyzer name and every settings dict key to
  match the Logic 2 UI *exactly* — not documented anywhere reliable, so the
  actual strings used here were read directly out of Saleae's open-source
  analyzer implementations, not guessed from the UI:
  [saleae/serial-analyzer](https://github.com/saleae/serial-analyzer)
  (`"Async Serial"`, `"Input Channel"`, `"Bit Rate (Bits/s)"`) and
  [saleae/async-rgb-led-analyzer](https://github.com/saleae/async-rgb-led-analyzer)
  (`"Addressable LEDs (Async)"`, `"LED Channel"`, `"LED Controller"`).
- `capture.wait()` has no built-in timeout — a command that never produces
  the triggering UART edge hangs forever. `capture_sweep.py` wraps it in a
  thread with a timeout (`config.CAPTURE_WAIT_TIMEOUT_SECONDS`).
- `logic2-automation` requires Logic 2 2.4.0+ and Python 3.8–3.10
  specifically — this project's venv should be built with a compatible
  interpreter, not whatever `python` defaults to system-wide.
- No true pre-trigger buffer exists in the API; `capture_sweep.py` fakes one
  by sleeping `PRE_ROLL_SECONDS` after arming (recording starts at arm
  time) before sending the triggering command.

## illuminate-control-2.0.html

A self-contained browser console (Web Serial for wired RS232, Web Bluetooth
for BLE) for manually exercising commands against real hardware, with a live
command-string preview before sending. Covers both the legacy 1.0 protocol
(gold) and `star_zone`'s `!`-prefixed enhancement commands (teal). **Keep
this in sync with whatever `firmware/star_zone/protocol.cpp` actually
implements** — it was originally built speculatively against the Illuminate
Enhanced PRD's draft `$`-prefixed command set (before `firmware/star_zone/`
existed) and drifting out of sync once already caused a real bug: a
since-removed "Shooting Star Pro" panel sent a command the firmware didn't
recognize, which then got misparsed by the legacy grammar's parser and
fired a shooting star for ~4 hours instead of a single shot.

Following a full command-by-command audit against `protocol.cpp` and
`illuminate_protocol.h`, the console also has:
- An "Off / Black" swatch and an "Overflow test" button (Starry Night —
  Colors) sending `c,0,0,0,0*` and the deliberately out-of-range
  `c,300,0,0,0*` (this field silently wraps instead of clamping).
- Shooting Star interval presets `1`/`2`/`10` alongside the star_zone
  seconds presets, specifically to exercise the legacy Illuminate 1.0
  `ShootingStarDuration` enum `{0,1,2,5,10}` minutes on real SS-firmware.
- "Omit field, use firmware default" checkboxes plus a manual number input
  next to the Smooth On/Off duration and Meteor Shower interval sliders —
  `!O`'s and `!W`'s optional fields and their (firmware-unlimited) upper
  ranges aren't reachable through a slider alone.
- A **Diagnostics** panel with a one-click resync-parser regression test
  (`c,1,2D,50*` sent as a single raw write) covering the same class of bug
  that caused the 4-hour shooting-star incident above.
- A prominent **"⏻ Power Off"** button in the header (`!O,0,300*`) plus a
  note on the Starry Night Effects panel — added after real confusion where
  setting color to black didn't turn off Random/Jamboree (those two ignore
  the commanded color entirely, by firmware design; `!O` is the actual,
  effect-independent off switch — see `firmware/star_zone/CLAUDE.md`).
- A **Decay** slider on the Shooting Star — Trail & Speed panel, sending
  `!L,zone,trail,speed,decay*` (always all 4 fields now — decay is only
  reachable when speed is also present on the wire, so the "no speed
  override" checkbox path now sends the `256` sentinel explicitly instead
  of omitting the field). Decoupling trail length from fade-curve steepness
  — see `firmware/star_zone/README.md`'s `!L` entry.
