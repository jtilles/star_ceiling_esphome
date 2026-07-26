# Project: Automated Saleae Logic 2 Capture Sweep for Fusion Synergy "Illuminate 1.0" LED Protocol

## Context

I'm reverse-engineering the LED data protocol used by a star ceiling controller board so I can
reimplement it from scratch in ESPHome/FastLED. I already have the full RS232 command protocol
figured out (documented below) and validated against real hardware over a wired UART at 9600
baud, 8N1. What I don't yet know is the exact byte-level protocol the board's zone microcontrollers
use to drive the physical LED strings (top row and bottom row of the star ceiling).

**Working assumption: the LED strings are WS2812/WS2812B-style ("NeoPixel") addressable LEDs** —
i.e. a single self-clocked data line per zone (no separate clock line), using the standard
800kHz-ish timing-encoded bit protocol (short high pulse = 0 bit, long high pulse = 1 bit, or
vice versa depending on exact variant — this is exactly what the capture data should reveal).
If the captures end up showing two active lines per zone with a much more regular/rectangular
clock-like signal on one of them, that would indicate an SPI-style protocol (APA102/SK9822)
instead — flag this back to me clearly if the data doesn't look like single-wire WS2812 timing,
don't just silently assume one or the other.

## What I want built

A Python script that automates a full sweep of every command in my known protocol, using a
connected Saleae logic analyzer to capture the resulting LED data-line activity for each one,
synchronized against the actual UART bytes sent (not a fixed delay), and exports everything to
clearly-named CSV files for later analysis.

## Hardware / tooling setup

- **Serial connection:** commands are sent over a USB-to-TTL serial adapter (5V logic level,
  9600 baud, 8N1, no flow control) connected to the board's UART header. Use `pyserial` for this.
- **Logic analyzer:** Saleae Logic 2, controlled via the official Automation API (the `saleae`
  Python package — gRPC-based, `pip install saleae`, or whatever the current correct package name
  is; please verify current install instructions, don't assume from training data since this API
  has changed versions before).
- **Capture channels (3 total):**
  1. UART RX line into the board (same signal the serial adapter is transmitting on) — this is
     the **trigger channel**, NOT the PC's own send timestamp. Trigger on a **falling edge**
     (UART idles high, start bit is a falling edge) to mark the exact moment each command begins
     arriving at the board.
  2. Top row zone LED data line
  3. Bottom row zone LED data line
- I will determine the correct Saleae device ID, channel-to-pin mapping, and digital sample rate
  myself first by running a manual capture through the Logic 2 desktop UI, then provide you the
  correct values to fill into the script's config section. **Do not hardcode a guessed sample
  rate** — WS2812 timing requires a high sample rate (order of tens of MHz) to resolve bit timing
  accurately; leave this as an obviously-marked config constant with a comment explaining why it
  matters and that I will supply the correct value after my manual test.
- Capture duration per command: **5 seconds**, plus a **500ms pre-roll of idle before the trigger**
  if the Saleae API supports pre-trigger buffering (check the API — if it only supports post-trigger
  captures, flag this limitation back to me rather than silently dropping the pre-roll requirement).

## Confirmed RS232 command protocol to sweep

All commands are ASCII strings, no line ending beyond what's specified per command. Sweep ALL of
the following, each as its own labeled capture:

### Starry Night Effects (single char, no terminator)
`T` (Twinkle), `F` (Firefly), `B` (Breeze), `R` (Random), `P` (Rain Drops), `J` (Jamboree)

### Starry Night Color Transitions (single char, no terminator)
`w` (Wipe), `f` (Fade), `r` (Rainbow), `b` (Blink)
NOTE: send one of these immediately preceded by a starry color command (e.g. `c,255,0,0,0*`
then `w`) since a transition command alone may not produce visible LED activity without an
active color set first — use your judgement here and note the sequencing choice in a comment.

### Starry Night Colors — format `c,R,G,B,W*`
Sweep the full named palette:
Red(255,0,0,0), Orange(255,50,0,0), Amber(255,150,0,0), Yellow(255,255,0,0), Green(0,255,0,0),
Light green(0,255,150,0), Lime(0,255,50,0), Aqua(0,255,255,0), Blue(0,0,255,0), Violet(150,0,255,0),
Purple(255,0,200,0), Pink(255,0,50,0), White(0,0,0,255), Warm White(202,218,255,0),
Natural White(255,246,237,0), Cool White(255,193,141,0)

### Star Dimmer — format `D,{pct}*`
Sweep: 10, 50, 100 (three sample points is enough, not every value 10-100)

### Shooting Star — format `{Zone},{R},{G},{B},{W},{time}*`
Zone: `X` (SS1), `Y` (SS2), `Z` (SS3). Sweep each zone with Red at time=1, plus one off command
per zone (`X,0,0,0,0,0*` etc). Skip this section gracefully (log a note, don't fail) if my board
is confirmed SP-firmware and won't respond — I'll tell you whether to include or skip this section
via a config flag, default to INCLUDED since I'll be testing against a separate SS-firmware unit.

### Ambient Light Effects — NOTE: no trailing `*` on these, unlike everything else above
`e,{speed}` (Fade), `k,{speed}` (Blink), `n` (No Effect) — sweep speed values 150 and 255 for
Fade and Blink (slow and fast ends of range)

### Ambient Light Colors — format `A,R,G,B,W*`
Same named palette as Starry Night colors above, same values, `A,` prefix instead of `c,`

### Ambient/Strip Dimmer — format `d,{pct}*`
Sweep: 10, 50, 100

## Script behavior requirements

1. **Config section at the top of the file**, clearly separated, containing: serial COM port,
   baud rate (default 9600), Saleae device ID (placeholder, marked TODO), channel assignments
   (placeholder, marked TODO), digital sample rate (placeholder, marked TODO with explanation),
   capture duration, pre-roll duration, output directory root, and a boolean flag for whether to
   include the shooting star sweep.

2. **Connect to both systems first and fail loudly** if either the serial port or the Saleae
   device can't be reached — don't start a long unattended sweep only to fail on command 3.

3. **For each command in the sweep list:**
   - Configure and arm the Saleae capture with the UART-line falling-edge trigger
   - Send the command string over serial
   - Wait for the capture to complete (5s post-trigger)
   - Export the capture to CSV
   - Name the output file clearly and unambiguously, e.g.
     `captures/starry_colors/color_warm_white.csv`,
     `captures/shooting_star/ss1_red_1min.csv`,
     `captures/ambient_effects/fade_speed_255.csv`
   - Log progress to the console (command being tested, file written) so I can watch it run
   - Add a **short fixed delay between commands** (make this configurable, default ~1s) so
     effects/colors don't visually bleed into each other and the board has time to settle

4. **Organize output into subfolders by category** (starry_effects/, starry_transitions/,
   starry_colors/, star_dimmer/, shooting_star/, ambient_effects/, ambient_colors/,
   strip_dimmer/) matching the sections above.

5. **Handle failures gracefully per-command** — if one capture or serial send fails, log it
   clearly and continue the sweep rather than aborting the whole run; keep a summary at the end
   of what succeeded/failed.

6. **Write a short README** alongside the script explaining: prerequisites (`pip install` list),
   how to fill in the config section after running a manual Logic 2 capture first, how to run the
   sweep, and what the output folder structure will look like.

## Open questions — ask me, don't guess silently

- Confirm the exact current `pip install` package name and minimum Logic 2 software version
  required for the Automation API before writing code that assumes an API shape — this has
  changed between beta and stable releases before, verify current docs.
- Confirm whether the Automation API supports specifying pre-trigger buffer time (I want 500ms
  of idle before the trigger point in each capture) — if not supported, tell me plainly what the
  closest available alternative is (e.g. starting the capture slightly before sending the command
  via a short software delay instead of true pre-trigger buffering) rather than silently omitting
  the pre-roll.
- Whether to also capture/log the literal UART bytes sent as a text sidecar file per capture
  (e.g. a `.txt` alongside each `.csv` recording the exact command string and timestamp sent) —
  I think this would help correlation later, use your judgment and flag your recommendation.

Please start by confirming the Saleae Automation API package/install details (web search if
needed, don't rely on possibly-stale training knowledge), then scaffold the script structure with
the config section and command-list data clearly separated from the capture/serial logic, and
check in with me before implementing the full capture loop.
