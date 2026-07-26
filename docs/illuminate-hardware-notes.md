# Fusion Synergy "Illuminate 1.0" — Hardware Reverse-Engineering Notes

Reverse-engineered pinout and architecture notes for the Fusion Synergy Illuminate 1.0 star
ceiling controller board, gathered via manual continuity tracing. Board uses 4x ATmega328P
microcontrollers (32-pin TQFP/QFN package) plus an ESP8266-family module (socketed, pin-compatible
with an HM-10 Bluetooth module or a NodeMCU-style ESP8266 dev board) and a MAX3232 for RS232-level
wired serial.

## Chip roles

| Chip | Role (inferred) |
|------|------------------|
| **Main** | Parses incoming commands (wireless via ESP8266, and/or wired via MAX3232). Broadcasts to zone MCUs. |
| **Top-Star** | Drives the top row of star ceiling LEDs. |
| **Bot-Star** | Drives the bottom row of star ceiling LEDs. |
| **RGBW** | Drives the ambient RGBW light output. Minimal signal tracing done — connections found were ground-plane only. |

Each MCU has its own independent ISP-6 programming header (SCK/MISO/MOSI/RESET/VCC/GND), consistent
with each chip carrying separately-flashable firmware (see note on SP vs SS firmware variants below).

## Main MCU — full pinout trace (ATmega328P, 32-pin package)

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

## Zone MCU findings (Bot-Star / Top-Star)

- **Pin 10 (PD6) on both Bot-Star and Top-Star drives their respective LED data line** (bottom
  row and top row respectively). PD6 has no hardware SPI/PWM peripheral tied to it — this is a
  **single-wire, bit-banged GPIO output**, consistent with WS2812/WS2812B ("NeoPixel")-style
  addressable LEDs rather than a clocked SPI protocol (APA102/SK9822 etc). LED protocol should
  still be confirmed via logic analyzer capture before assuming timing details.
- Each zone MCU has its own ISP-6 header — the SCK/MISO/MOSI-labeled headers near each zone chip
  are **programming headers, not an SPI bus to the LEDs.**
- Bot-Star pin 30 (its own hardware RXD) ties into the same net as Main's pin 30 and the ESP8266's
  TX — see "Serial paths" below.

## Serial paths (partially resolved — one open question remains)

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

## RGBW MCU

Only ground-plane connections found from Main (pins 3, 5, 21, all GND per datasheet) — no
meaningful signal path traced yet. Not pursued further; logic analyzer captures of the ambient
light output are a more direct way to characterize this chip's LED protocol than continued manual
tracing.

## Firmware note (confirmed with designer)

Per direct confirmation from Fusion Synergy's designer, **Main ships with one of two mutually
exclusive firmware loads**: one for static "SP" (star panel) zones, one for motorized/addressable
"SS" (shooting star) zones. A single board only supports one mode; shooting star RS232 commands
are silently accepted and ignored on SP-firmware units. Each zone MCU likely has a matching
firmware variant for the same reason.

## Suggested next steps

- Logic analyzer capture on Bot-Star/Top-Star pin 10 (LED data lines) during known commands to
  confirm WS2812-style timing and determine RGB(W) byte order / bit depth.
- Continuity check between Main pin 12 and pin 31 to resolve the TX-source jumper question above.
- Confirm whether Top-Star's hardware RXD is on the same broadcast bus as Main/Bot-Star (pin 30
  net), to determine if all three USART-bearing chips listen to the same wireless command stream.
