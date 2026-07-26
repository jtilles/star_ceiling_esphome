# Saleae capture sweep

Automates a full sweep of every known Illuminate 1.0 RS232 command against
real hardware, capturing the resulting LED data-line activity with a Saleae
logic analyzer, synchronized to the actual UART bytes sent (not a fixed
delay). Built from the spec in
[`docs/prompts/claude-code-saleae-prompt.md`](../prompts/claude-code-saleae-prompt.md).

Goal: figure out the byte-level protocol the board's zone MCUs use to drive
the physical LED strings. Confirmed via manual capture: WS2812B single-wire
timing.

Each capture gets up to 3 protocol analyzers attached and is exported as one
combined **decoded** data table CSV (actual bytes/pixel values with
timestamps), not raw digital timing:
- UART RX line -> `Async Serial` analyzer (9600 8N1) -- always captured.
- Top row (P-TOP) LED line -> `Addressable LEDs (Async)` analyzer (WS2812B)
  -- captured when `config.INCLUDE_TOP_ROW` is `True`.
- Bottom row (P-BOTTOM) LED line -> `Addressable LEDs (Async)` analyzer
  (WS2812B) -- captured when `config.INCLUDE_BOTTOM_ROW` is `True`.

`config.INCLUDE_TOP_ROW` / `INCLUDE_BOTTOM_ROW` / `INCLUDE_AMBIENT_SWEEP` let
you narrow a run down: the two row flags control which LED lines get
captured (independent of which commands are sent), and `INCLUDE_AMBIENT_SWEEP`
skips the `ambient_effects`/`ambient_colors`/`strip_dimmer` command
categories entirely (e.g. no ambient strip wired up yet). All default `True`.

## Prerequisites

1. Logic 2 desktop app installed (2.4.0+), running, with your analyzer
   plugged in.
2. Python 3.8, 3.9, or 3.10 (the `logic2-automation` package's supported
   range at time of writing -- newer Pythons may not have a compatible
   wheel/build).
3. `pip install -r requirements.txt`
4. Enable the Automation server: Logic 2 -> Preferences -> scroll to the
   bottom -> enable. It listens on `127.0.0.1:10430` by default.
5. Wire up 3 digital channels:
   - UART RX line into the board (same signal your serial adapter
     transmits) -- this is the **trigger channel**.
   - Top row (P-TOP) zone LED data line.
   - Bottom row (P-BOTTOM) zone LED data line.
6. Connect your USB-to-TTL serial adapter to the board's UART header (5V
   logic, 9600 8N1, no flow control).

## One-time setup: fill in config.py

1. `python list_saleae_devices.py` -- copy the printed `device_id` of your
   real device into `config.SALEAE_DEVICE_ID`.
2. In the Logic 2 UI, start one **manual** capture with all 3 channels
   enabled, send any Starry Night color command by hand over serial, and
   confirm you see line activity. Use this capture to determine:
   - Which channel numbers you wired RX / top row / bottom row to ->
     `config.UART_RX_CHANNEL` / `TOP_ROW_CHANNEL` / `BOTTOM_ROW_CHANNEL`.
   - A sample rate that resolves WS2812-style timing cleanly (order of tens
     of MHz -- check the Device Settings panel for valid rates at your
     enabled channel count) -> `config.DIGITAL_SAMPLE_RATE_HZ`.
3. Fill in `config.SERIAL_PORT` (e.g. `COM5`).

Don't skip step 2 -- an insufficient sample rate silently produces garbage
timing data that still looks like a plausible capture.

## Running the sweep

```
python capture_sweep.py
```

It connects to both the serial port and Logic 2 first and fails loudly if
either is unreachable, so you don't discover a wiring problem 40 commands
in. Then it walks every command from `commands.build_sweep()`, printing
progress as it goes, and prints a pass/fail summary at the end. A failed
capture is logged and the sweep continues rather than aborting.

## Output layout

```
captures/
  starry_effects/twinkle.csv          (+ twinkle.json sidecar)
  starry_effects/firefly.csv
  ...
  starry_transitions/wipe.csv
  starry_colors/color_warm_white.csv
  star_dimmer/dimmer_50.csv
  shooting_star/ss1_red_1min.csv
  shooting_star/ss1_off.csv
  ambient_effects/fade_speed_255.csv
  ambient_colors/color_blue.csv
  strip_dimmer/dimmer_100.csv
```

Each `.csv` is a decoded data table export (all 3 analyzers combined into one
file -- an "Analyzer" column, using the labels `uart_rx`/`top_row`/
`bottom_row`, tells you which line each row came from). UART rows show the
decoded command bytes; LED rows show decoded WS2812B pixel values (RGB per
LED), not raw bit timing. Each capture has a `.json` sidecar with the same
base name recording the literal command string(s) sent, an ISO8601 UTC
timestamp, and the capture parameters in effect -- for correlating a capture
back to exactly what was sent later during analysis.

## Design decisions / open questions from the spec

**Saleae package & versions** (verified against the actual package source on
GitHub, not just docs, since the API has changed shape before):
`pip install logic2-automation` (>=1.0.0), requires Logic 2 2.4.0+ and
Python 3.8-3.10. Source: [saleae/logic2-automation](https://github.com/saleae/logic2-automation).

**Pre-trigger buffering**: not supported by the Automation API.
`DigitalTriggerCaptureMode` only exposes `after_trigger_seconds` (recording
length *after* the trigger fires) and `trim_data_seconds` (keeps only the
last N seconds of the *whole* buffer, trimmed from the end) -- there is no
parameter for "N seconds before the trigger." The workaround implemented
here: `start_capture()` arms the capture and returns immediately (recording
begins at that point), then `capture_sweep.py` sleeps
`config.PRE_ROLL_SECONDS` before sending the command that produces the
triggering UART falling edge. Because recording is continuous from arm time,
that sleep becomes genuine leading idle-line data in the export -- not a
literal pre-trigger buffer feature, but it achieves the same practical
result. `TRIM_DATA_SECONDS` trims the tail of the exported window to roughly
`pre-roll + after-trigger` seconds so files don't carry an unbounded amount
of idle time from before the sleep started.

**UART sidecar files**: implemented as one `.json` per capture (see Output
layout above), recommended over a plain `.txt` because the structured
command/timestamp/parameter data is meant to be consumed programmatically
during later analysis, not just read by eye.

**Starry Night color transitions need an active color**: `w`/`f`/`r`/`b`
transition commands are sent with an uncaptured `c,255,0,0,0*` (Red)
priming command first (see `commands.STARRY_TRANSITION_PRE_COMMAND`), so the
transition has something to visibly act on. This priming step happens
*before* the capture is armed, so it doesn't pollute the trigger -- only the
actual transition command byte produces the triggering falling edge.

**Analyzer names/settings**: `add_analyzer()` requires the analyzer name and
every settings dict key to match the Logic 2 UI *exactly*, and that's not
fully documented anywhere -- so instead of guessing from the UI, the exact
strings in `config.py` (`"Async Serial"`, `"Addressable LEDs (Async)"`,
`"Input Channel"`, `"LED Channel"`, `"LED Controller"`, `"WS2812B"`) were
read directly out of Saleae's own open-source analyzer implementations:
[saleae/serial-analyzer](https://github.com/saleae/serial-analyzer) and
[saleae/async-rgb-led-analyzer](https://github.com/saleae/async-rgb-led-analyzer).
Only the channel-assignment and controller-type settings are passed
explicitly; every other Async Serial setting already defaults to this
protocol's framing (8 bits, 1 stop bit, no parity, LSB first, non-inverted,
Normal mode), so there was nothing to override there.

## If the captures don't look like WS2812 timing

The working assumption is single-wire, self-clocked WS2812/WS2812B timing on
both LED data lines. If a capture instead shows two active lines per zone
with a regular, clock-like signal on one of them, that points to an
SPI-style protocol (APA102/SK9822) instead -- stop and re-check before
building any decoder around the WS2812 assumption.
