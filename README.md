# esphome-illuminate

An ESPHome external component for the Fusion Synergy "Illuminate 1.0" star ceiling /
ambient lighting controller, so it can be driven natively from Home Assistant over a
direct UART connection instead of the proprietary Bluetooth phone app.

<img src="docs/images/illuminate1.0_render.png" alt="Illuminate 1.0 controller enclosure, front and rear panel" width="500">

> **Built with [Claude Code](https://claude.com/claude-code).** The RS232 protocol was
> reverse-engineered and validated against real hardware by the repo owner; the ESPHome
> component (Python codegen, C++ classes, and this README) was written by Claude Code
> against that protocol and the manual linked below, then verified by actually compiling
> it with `esphome compile`. Review the code with that in mind.

**Status:** the protocol layer, hub component, both light platforms (Ambient Light and
Starry Night), the Starry Night color-transition select, and the Shooting Star light are
all implemented and pass `esphome compile` against the example config. Starry Night
effects/colors and both dimmers have been bench-validated against real hardware over a
wired UART connection; Ambient Light effects/colors are implemented per the manual but
not yet bench-tested (see "What's actually been tested" below). Shooting Star is
implemented per the manual but is **not functional on SP-firmware units** — see below.
A separate, standalone replacement firmware for the Top-Star/Bot-Star chips themselves
(adding real Shooting Star support on any unit, plus brightness/speed/palette
enhancements) lives in `firmware/star_zone/` — see
["Top/Bot-Star replacement firmware"](#topbot-star-replacement-firmware) below; it has
been bench-flashed and iterated against real hardware.

The component now also speaks that firmware's `!`-prefixed enhancement command set — see
["Driving the star_zone enhancements"](#driving-the-star_zone-enhancements).

## What this hardware is

The Illuminate 1.0 is a 4x-ATmega328 controller board for fiber-optic/LED "star ceiling"
and ambient lighting installations, normally controlled from a phone app over Bluetooth
(an HM-10 BLE UART-passthrough module) or a wired RS232 connector (via an onboard
MAX3232 level shifter). This component bypasses both: it talks directly to the TTL-level
UART pins the HM-10 module normally occupies, from an ESP8266 dev board plugged into the
same socket.

Official manual: [`docs/IlluminateUserManual.pdf`](docs/IlluminateUserManual.pdf).

## SP vs. SS firmware — read this before wiring up Shooting Star

The board's "Main" MCU ships with one of two mutually exclusive firmware loads,
confirmed directly with Fusion Synergy's designer:

- **SP firmware** — static star panels. Supports Starry Night and Ambient Light.
- **SS firmware** — motorized/addressable "shooting star" zones. Supports Shooting Star.

**A single unit only supports one or the other.** Shooting Star commands are silently
accepted and ignored on SP-firmware units — the hardware won't error, log, or otherwise
indicate anything is wrong, it simply won't move. If your `shooting_star` light entity in
Home Assistant appears to do nothing, this firmware mismatch is almost certainly why.
Omit the `type: shooting_star` light from your config entirely if you only have an
SP-firmware board; there's nothing else to disable.

## Wiring

<img src="docs/images/board_wo_bluetooth_or_wifi.jpg" alt="Illuminate 1.0 Main board with the HM-10/ESP8266 wireless module removed, showing the header socket" width="450">

The board above has its wireless module (HM-10 BLE or an ESP8266, depending on SKU)
removed, showing the header socket this component connects to instead. Connect the
ESP8266 directly to that header (labeled `D0`-`D8` or similar depending on board
silkscreen revision):

| ESP8266 | Illuminate board |
|---------|-------------------|
| GND     | GND               |
| TX      | RX (board's UART RX, feeds the Main MCU) |
| RX      | TX (board's UART TX)                     |

9600 baud, 8N1, no flow control, no parity. This UART bus should be dedicated to the
`illuminate:` hub — don't attach any other `uart` device to the same bus/pins.

**5V logic level warning:** the board runs its UART at 5V TTL levels. The board's TX
line, wired directly into your ESP8266's RX pin, will drive 5V into a pin many ESP8266
dev boards only tolerate up to 3.3V. **Verify your specific dev board's RX pin is 5V
tolerant before wiring this directly** (some ESP-12E NodeMCU clones are, many aren't —
check your board's datasheet), or add a level shifter / simple resistor voltage divider
on that line. ESP8266 TX → board RX is fine without shifting either way, since the
board's RX threshold accepts 3.3V logic as a valid high.

## Daisy-chaining an SP board and an SS board

To get both Starry Night/Ambient (SP firmware) and Shooting Star (SS firmware) control
from one ESP8266, run **two `uart:` buses and two `illuminate:` hubs** in one YAML — the
ESP8266 has enough spare GPIOs for a second software/hardware UART pair:

```yaml
uart:
  - id: uart_sp
    tx_pin: GPIO1
    rx_pin: GPIO3
    baud_rate: 9600
  - id: uart_ss
    tx_pin: GPIO14
    rx_pin: GPIO12
    baud_rate: 9600

illuminate:
  - id: hub_sp
    uart_id: uart_sp
  - id: hub_ss
    uart_id: uart_ss

light:
  - platform: illuminate
    type: starry_night
    illuminate_id: hub_sp
    ...
  - platform: illuminate
    type: shooting_star
    illuminate_id: hub_ss
    zone: zone_1
    ...
```

The `illuminate:` hub supports multiple instances (`MULTI_CONF`) specifically for this.
This has not yet been bench-tested against two physical boards on one ESP8266 (the
repo owner is still acquiring the second, SS-firmware board) — treat it as
implemented-per-design rather than field-validated until confirmed.

## Design notes / tradeoffs

- **Turning a light "off":** the manual has no documented power on/off command for
  Starry Night or Ambient Light (only effects, color, and a 10-100% dimmer). "Off" is
  approximated by sending the RGBW color command with all channels zeroed. This leaves
  the dimmer and any active effect at their last commanded value on the hardware — if
  that turns out to be wrong on real hardware, it would fail the same way the Shooting
  Star investigation did (looks correct, silently doesn't do what you'd expect).
- **Starry Night color transitions** (Wipe/Fade/Rainbow/Blink) are modeled as a companion
  `select:` entity (`illuminate_color_transition`) rather than folded into the Starry
  Night light's `effects:` list. The manual describes them separately from the six
  animation effects, and they're a modifier layered on top of whatever color/effect is
  currently active, not an alternate mode — folding all ten into one ESPHome effect list
  would force choosing only one of e.g. "Twinkle" or "Wipe" at a time in HA, when the
  hardware lets both be active together.
- **Ambient effect speed** (150-255, for Fade/Blink) is a fixed config-time parameter on
  each effect entry in YAML (like core ESPHome effects such as `pulse` carry a fixed
  `transition_length`), not a live HA control — HA's effect picker only lets you choose
  *which* effect is active, not tune a running effect's parameters, so a runtime speed
  slider wouldn't be reachable from the HA UI regardless.
- **Shooting Star** is modeled as a `light` rather than a `button` + `number`/`select`.
  A shot is fundamentally "a color, fired on a cadence," which maps naturally onto a
  light's color + on/off, and gets HA's color picker for free. The tradeoff: the firing
  *interval* doesn't fit HA's light data model at all (it's not brightness or a
  transition), so it's a fixed config-time value (`interval:`) per light entry rather
  than adjustable per-shot from the HA UI — add one `illuminate` light entry per
  zone/interval combination you want available if you need more than one at runtime.
  Turning the light off sends the explicit `{Zone},0,0,0,0,0*` shorthand, canceling the
  zone immediately.
- **`interval:` is seconds *between* shots, not a shot length.** The original manual
  documented this wire field as `0/1/2/5/10` meaning minutes of *continuous* firing;
  `star_zone` firmware reinterprets the same field position as an interval — fire one
  streak, then another every N seconds, until sent `0`. Real bench testing found "fire
  predictably every N seconds" far more useful than "run continuously for N minutes," so
  the field's meaning changed even though its position didn't. The component follows the
  firmware, so `interval:` takes any time period and the full 16-bit range (1s-65535s).
- **The enhancement commands are exposed as hub methods, not as new platforms.** Every
  `!` command is a public `send_*` on `IlluminateHub`, driven from YAML lambdas on stock
  `template` number/switch/select/button entities (see `cooper_ceiling.yaml`). Eleven
  near-identical codegen platforms would have been a lot of machinery to give each
  command a bespoke `platform:` name, and it would have locked each one into a single
  entity type; a public hub API leaves the choice of entity — and of which commands to
  surface at all — to the config.
- **Brightness floor:** the hardware's dimmers only go down to 10%, not 0. ESPHome's
  0.0-1.0 brightness range is mapped linearly onto 10-100% (`pct = round(10 + b*90)`)
  rather than snapping straight to the floor, so the full HA brightness slider range
  remains meaningful.

## What's actually been tested

Confirmed against real hardware over a wired UART connection: Starry Night effects,
Starry Night colors, and the star dimmer. Implied by protocol symmetry but **not yet
bench-tested**: Ambient Light effects/colors, the ambient dimmer, and the four color
transitions. Shooting Star is confirmed **not functional** on a stock SP-firmware unit
(expected — see "SP vs. SS firmware" above) and has never been tested against a real
SS-firmware board; it *is* functional against `firmware/star_zone/`, which is what the
zone chip in this repo owner's ceiling now runs.

The `!` enhancement commands are generated from the ranges documented in
`firmware/star_zone/README.md` and their wire shapes were checked against that firmware's
own parser (`firmware/star_zone/src/protocol.cpp`), but the ESPHome-side entities driving
them have not yet been exercised end-to-end against the hardware.

## Driving the star_zone enhancements

`firmware/star_zone/` adds a `!`-prefixed command set on top of the legacy grammar (full
per-field reference in [that firmware's README](firmware/star_zone/README.md)). Each one
is a public method on the hub, so any YAML lambda can reach it:

| Command | Hub method | Wired up in `cooper_ceiling.yaml` as |
|---|---|---|
| `!M,pct*` | `send_master_brightness(pct)` | number — Star Ceiling Master Brightness |
| `!B,pct*` | `send_shooting_star_brightness(pct)` | number — Shooting Star Brightness |
| `!S,speed*` | `send_effect_speed(speed)` | number — Star Ceiling Effect Speed |
| `!F,ms*` | `send_color_transition_duration(ms)` | number — Star Ceiling Transition Duration |
| `!L,zone,trail,speed,decay*` | `send_shooting_star_tuning(zone, trail, speed, decay)` | numbers — Shooting Star Trail/Speed/Decay |
| `!O,{0\|1},ms*` | `send_power(on, ms)` | switch — Star Ceiling Power |
| `!I,{0\|1}*` | `send_identify(on)` | switch — Star Ceiling Identify Mode |
| `!W,{0\|1},s*` | `send_meteor_shower(on, s)` | switch + number — Meteor Shower |
| `!C,id*` | `send_palette(palette)` | select — Star Ceiling Palette |
| `!N*` / `!V*` | `send_identify_next()` / `send_identify_previous()` | buttons — Identify Next/Previous LED |

Three things worth knowing before adapting this to your own config:

- **These are safe to leave in a stock-board config.** Stock zone firmware ignores any
  command starting with `!`, so the entities still validate, still send, and simply do
  nothing — they degrade to no-ops rather than erroring.
- **`!L` is all-or-nothing.** It fully re-specifies a zone's tuning; an omitted trailing
  field *resets* that field rather than leaving it alone. That's why the three tuning
  numbers all route through one `apply_shooting_star_tuning` script that re-sends the
  complete trail/speed/decay triple whenever any one of them changes, instead of each
  number sending its own partial `!L`. The `speed` field's "no override" sentinel is
  `256` (`protocol::SHOOTING_STAR_SPEED_INHERIT`), which is why that slider runs to 256
  rather than 255 and why the field is a `uint16_t`.
- **Power on/off is not the same as setting the color to black.** Random and Jamboree
  generate their own per-pixel colors and ignore the commanded color entirely, so black
  doesn't turn them off. `!O` is a post-render output multiplier applied after every
  effect, so nothing can bypass it. The firmware deliberately does not persist power
  state and always boots ON, which is why the switch uses `restore_mode: ALWAYS_ON`.

One behavior change to be aware of if you were already using `D`: on `star_zone`, the
star dimmer now scales the Starry Night layer **only**, and is applied before streaks are
blended in. Streaks are scaled by `!B` instead. That's what allows a dim ambient starfield
under full-brightness shooting stars — impossible before, when `D` dimmed the composited
frame and took both down together.

## Repo layout

```
components/illuminate/
├── __init__.py                 # illuminate: hub config + shared protocol enums
├── illuminate.h / .cpp         # IlluminateHub: Component + UARTDevice
├── illuminate_protocol.h / .cpp  # pure command-string builders, no ESPHome deps
├── light/
│   ├── __init__.py             # platform: illuminate under light: (type: starry_night
│   │                           # / ambient / shooting_star, see note below)
│   ├── starry_night_light.h / .cpp
│   ├── ambient_light.h / .cpp
│   └── shooting_star_light.h / .cpp
└── select/
    ├── __init__.py             # platform: illuminate under select: (color transition)
    └── illuminate_color_transition_select.h / .cpp

firmware/star_zone/           # standalone Top-Star/Bot-Star replacement firmware
├── platformio.ini
└── src/
    ├── main.cpp               # setup()/loop()
    ├── controller.h / .cpp    # StarZoneController: all runtime state + render pipeline
    ├── protocol.h / .cpp      # serial command parser (legacy + new "!" grammar)
    ├── star_effects.h / .cpp  # Twinkle/Firefly/Breeze/Random/Rain Drops/Jamboree
    ├── shooting_star.h / .cpp # new Shooting Star implementation
    ├── palette.h / .cpp       # built-in palettes for palette-driven twinkle
    ├── color_utils.h / .cpp   # scale/lerp/blend/hue helpers
    └── config.h                # pins, LED count, baud rate
```

Note: ESPHome resolves a platform as `esphome.components.<platform>.<domain>`, so a
single `platform: illuminate` light entry can only live at `components/illuminate/light/`
— there's no way to give Shooting Star its own `platform:` name without giving it its own
top-level component directory outside `illuminate/`. Rather than do that, Shooting Star
is a third `type:` option on the same `platform: illuminate` light entry as Starry Night
and Ambient (see `light/__init__.py`), distinguished by a required `type:` config key.

## Development

The pure protocol layer has no ESPHome or Arduino dependencies and can be built/tested
with a desktop compiler:

```sh
cd tests
g++ -std=c++17 -Wall -Wextra -I ../components/illuminate \
    test_illuminate_protocol.cpp ../components/illuminate/illuminate_protocol.cpp \
    -o test_illuminate_protocol
./test_illuminate_protocol
```

`tests/test_config.yaml` is a config exercising every platform/type combination, useful
for `esphome config` / `esphome compile` sanity checks against this repo's
`external_components:` local source while developing.

See `examples/example.yaml` for a config meant to be copied into a real device's YAML.

## Top/Bot-Star replacement firmware

`firmware/star_zone/` is a standalone PlatformIO firmware project that replaces the
stock Top-Star and Bot-Star ATmega328P chips with new firmware adding working Shooting
Star support (the stock chips have none — see "SP vs. SS firmware" above) plus
brightness/speed/palette/identify-mode enhancements, while staying wire-compatible with
the protocol this ESPHome component already speaks. **Main and RGBW stay on their stock
firmware** — only the two zone chips get reflashed, with the exact same image on both
(matching the original board's own architecture — Top-Star and Bot-Star run
byte-identical stock firmware, per `docs/illuminate-firmware-notes.md`). See
[`firmware/star_zone/README.md`](firmware/star_zone/README.md) for the full command
grammar, design notes, and open questions to verify against real hardware before
trusting it — LED wire byte order in particular.

**Status:** compiles clean (`pio run`, 0 warnings, ~40% flash / ~24% RAM on an
ATmega328P). Not yet flashed to or bench-tested against a physical chip — the effects,
Shooting Star behavior, and RGBW byte order are all best-effort designs built from the
reverse-engineering docs, not validated hardware behavior. Treat this the same way the
rest of this repo treats "implemented per spec, not yet bench-tested" claims.

### Compile

```sh
cd firmware/star_zone
pio run
```

Requires [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html)
(`pip install platformio`, or use the bundled CLI from the VS Code extension). The first
run downloads the `atmelavr` platform toolchain and the Adafruit NeoPixel library
automatically. Output: `.pio/build/star_zone/firmware.hex`.

### Install (flash via ISP)

Each zone chip has its own independent ISP-6 header (`docs/illuminate-hardware-notes.md`)
— programming does **not** go through the wireless/RS232 UART path this repo's ESPHome
component uses. You need a physical ISP programmer (USBasp, an Arduino running
`ArduinoISP`, an AVRISP mkII, etc.) wired to that header.

1. Wire your programmer to the zone chip's ISP-6 header (MOSI/MISO/SCK/RESET/VCC/GND).
2. Edit `firmware/star_zone/platformio.ini`'s `upload_protocol` (and `upload_flags` if
   needed) to match your programmer — it defaults to `usbasp`. Common alternatives:
   `stk500v1` (Arduino-as-ISP; also set `upload_speed = 19200` and
   `upload_flags = -P$UPLOAD_PORT`), `avrispmkii`.
3. Flash:
   ```sh
   cd firmware/star_zone
   pio run --target upload
   ```
4. Repeat on the second zone chip — same `.hex`, same command, just move the
   programmer's ISP connection to the other chip's header.

Fuses aren't touched by a normal upload. The board definition (`pro16MHzatmega328`)
assumes the same external-16MHz-crystal fuse configuration the stock chip already runs
under (matches the confirmed 16MHz crystal in `docs/illuminate-hardware-notes.md`), so
this should be a non-issue on a chip that's already running the stock firmware. If you do
need to (re)write fuses explicitly: `pio run --target fuses`.

**Before you commit to reflashing a physical chip:** dump and archive its existing
firmware first (`binaries/original_dumps/dump_chip.ps1` in this repo covers that with
`avrdude -p m328p`) — this firmware hasn't been bench-validated yet, and ISP flashing
overwrites the stock image with no built-in way back short of that dump.

## Hardware Reverse-Engineering Notes

Reverse-engineered pinout and architecture notes for the Fusion Synergy Illuminate 1.0 star
ceiling controller board, gathered via manual continuity tracing. Board uses 4x ATmega328P
microcontrollers (32-pin TQFP/QFN package) plus an ESP8266-family module (socketed, pin-compatible
with an HM-10 Bluetooth module or a NodeMCU-style ESP8266 dev board) and a MAX3232 for RS232-level
wired serial.

### Chip roles

| Chip | Role (inferred) |
|------|------------------|
| **Main** | Parses incoming commands (wireless via ESP8266, and/or wired via MAX3232). Broadcasts to zone MCUs. |
| **Top-Star** | Drives the top row of star ceiling LEDs. |
| **Bot-Star** | Drives the bottom row of star ceiling LEDs. |
| **RGBW** | Drives the ambient RGBW light output. Minimal signal tracing done — connections found were ground-plane only. |

Each MCU has its own independent ISP-6 programming header (SCK/MISO/MOSI/RESET/VCC/GND), consistent
with each chip carrying separately-flashable firmware (see note on SP vs SS firmware variants below).

### Main MCU — full pinout trace (ATmega328P, 32-pin package)

| Pin | Datasheet fn | Connection found | Notes |
|-----|--------------|-------------------|-------|
| 1 | PD3 (INT1) | → Bot-Star pin 24 | Bespoke bit-banged inter-chip link |
| 2 | PD4 | → Bot-Star pin 23 | Bespoke bit-banged inter-chip link |
| 3 | GND | GND | |
| 4 | VCC | not directly traced | Likely buried via under package |
| 5 | GND | GND | |
| 6 | VCC | not directly traced | Likely buried via under package |
| 7 | PB6 (XTAL1) | 16MHz crystal | |
| 8 | PB7 (XTAL2) | 16MHz crystal | |
| 9 | PD5 | NC | |
| 10 | PD6 | NC | |
| 11 | PD7 (AIN1) | → jumper (under ESP socket) → MAX232 pin 9 (R2OUT) | Bit-banged software-serial RX candidate — see "Serial paths" below |
| 12 | PB0 | → jumper (under ESP socket) → MAX232 pin 10 (T2IN) | Bit-banged software-serial TX candidate — see "Serial paths" below |
| 13 | PB1 | NC | |
| 14 | PB2 (SS) | NC | |
| 15 | PB3 (MOSI) | ISP header | Hardware SPI reserved for programming, not used as a data bus |
| 16 | PB4 (MISO) | ISP header | |
| 17 | PB5 (SCK) | ISP header | |
| 18 | AVCC | onboard status LED | |
| 19 | ADC6 | NC | Analog-only pin, no digital function available |
| 20 | AREF | 0.1µF cap (C30) | Standard AREF decoupling |
| 21 | GND | reported NC | Likely another buried via, per datasheet this is GND |
| 22 | ADC7 | NC | |
| 23 | PC0 (ADC0) | pushbutton SW4 | |
| 24 | PC1 (ADC1) | T1 (8050 NPN transistor) base | GPIO driving a BJT switch |
| 25 | PC2 (ADC2) | via, untraced | Unresolved |
| 26 | PC3 (ADC3) | → Top-Star pin 25 | Bespoke bit-banged inter-chip link |
| 27 | PC4 (ADC4/SDA) | → Top-Star pin 24 | Used as plain GPIO, not hardware I2C |
| 28 | PC5 (ADC5/SCL) | → Top-Star pin 26 | Used as plain GPIO, not hardware I2C |
| 29 | PC6 (RESET) | ESP socket reset pin | |
| 30 | PD0 (RXD, hardware USART) | → Bot-Star pin 30 (its own hardware RXD) **and** → ESP8266 NodeMCU TX | Shared broadcast bus — see "Serial paths" below |
| 31 | PD1 (TXD, hardware USART) | → MAX232 pin 10 (T2IN) | See "Serial paths" — conflicts with pin 12's traced path to the same MAX232 pin |
| 32 | VCC | not directly traced | |

### Zone MCU findings (Bot-Star / Top-Star)

- **Pin 10 (PD6) on both Bot-Star and Top-Star drives their respective LED data line** (bottom
  row and top row respectively). PD6 has no hardware SPI/PWM peripheral tied to it — this is a
  **single-wire, bit-banged GPIO output**, consistent with WS2812/WS2812B ("NeoPixel")-style
  addressable LEDs rather than a clocked SPI protocol (APA102/SK9822 etc). LED protocol should
  still be confirmed via logic analyzer capture before assuming timing details.
- Each zone MCU has its own ISP-6 header — the SCK/MISO/MOSI-labeled headers near each zone chip
  are **programming headers, not an SPI bus to the LEDs.**
- Bot-Star pin 30 (its own hardware RXD) ties into the same net as Main's pin 30 and the ESP8266's
  TX — see "Serial paths" below.

### Serial paths (partially resolved — one open question remains)

The board appears to implement **two logically separate serial input paths**, since the
ATmega328P only has one hardware USART and this board needs to service both a wireless module and
a wired RS232 connector:

1. **Wireless / broadcast path — hardware USART (Main pins 30/31):**
   Main's hardware RXD (pin 30) is tied directly to the ESP8266 module's TX, and *also* to
   Bot-Star's own hardware RXD (pin 30) on the same net. This is a **shared broadcast bus** —
   commands arriving from the ESP8266 module appear to be received simultaneously by both Main
   and Bot-Star's hardware USARTs. (Top-Star's equivalent pin has not yet been traced to confirm
   whether it's on this same bus.)

2. **Wired RS232 path — likely bit-banged software serial (Main pins 11/12, PD7/PB0):**
   These route through a jumper footprint located *underneath the ESP8266/Bluetooth module socket*
   before reaching the MAX232 (pin 9 = R2OUT / RS232 receive side, pin 10 = T2IN / RS232 transmit
   side). PD7/PB0 have no hardware USART function, so this is presumably an Arduino
   `SoftwareSerial`-style bit-banged implementation, kept independent of the wireless hardware
   USART.

**Open conflict, not yet resolved:** Main's hardware TXD (pin 31) was *also* traced to MAX232 pin
10 — the same net reached via the pin 12 jumper path above. Both can't drive that node
simultaneously. Leading theory: **the under-ESP jumper header is a TX-source selector**, physically
bridging either the bit-banged software TX (pin 12) or the hardware TX (pin 31) onto the MAX232
input, making the wired-serial TX source configurable rather than fixed. Needs a direct continuity
check between pin 12 and pin 31 to confirm whether they're currently the same net (jumper bridged)
or genuinely separate paths.

### RGBW MCU

Only ground-plane connections found from Main (pins 3, 5, 21, all GND per datasheet) — no
meaningful signal path traced yet. Not pursued further; logic analyzer captures of the ambient
light output are a more direct way to characterize this chip's LED protocol than continued manual
tracing.

### Firmware note (confirmed with designer)

Per direct confirmation from Fusion Synergy's designer, **Main ships with one of two mutually
exclusive firmware loads**: one for static "SP" (star panel) zones, one for motorized/addressable
"SS" (shooting star) zones. A single board only supports one mode; shooting star RS232 commands
are silently accepted and ignored on SP-firmware units. Each zone MCU likely has a matching
firmware variant for the same reason.

### Suggested next steps

- Logic analyzer capture on Bot-Star/Top-Star pin 10 (LED data lines) during known commands to
  confirm WS2812-style timing and determine RGB(W) byte order / bit depth.
- Continuity check between Main pin 12 and pin 31 to resolve the TX-source jumper question above.
- Confirm whether Top-Star's hardware RXD is on the same broadcast bus as Main/Bot-Star (pin 30
  net), to determine if all three USART-bearing chips listen to the same wireless command stream.

## License

MIT — see [LICENSE](LICENSE).
