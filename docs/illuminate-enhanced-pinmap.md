# Illuminate Enhanced — Single-MCU Pin Map (ATmega328P "Main")

Proposed pin reassignment for consolidating Main + Top-Star + Bot-Star + RGBW onto a single
ATmega328P ("Main"), driving all outputs directly. Verified against the ATmega328P 32-pin
TQFP/QFN datasheet pin functions and the existing trace in `illuminate-hardware-notes.md`.

## Design constraints driving this map

1. **Backward compatibility is required** — existing wired RS232 (pins 11/12 → MAX232) and
   wireless (pins 30/31 hardware USART → ESP module) paths must be preserved exactly as-is, since
   this needs to work as a drop-in replacement without requiring any change to what talks to it.
2. **ISP-6 programming header (pins 15/16/17/29 + VCC/GND) must stay reserved** — do not repurpose
   MOSI/MISO/SCK even though they're otherwise idle, since in-circuit reprogramming needs them free
   of conflicting peripherals.
3. **4x Ambient RGBW channels need true hardware PWM** — these should NOT be bit-banged software
   PWM, to avoid flicker/jitter competing with WS2812 bit-banged timing on the same chip.
4. **1x combined Star WS2812 output** — Top row and Bottom row (8 LEDs each) are wired in series
   into a single 16-LED array driven from one data pin, rather than two separate outputs. Shooting
   Star is a **firmware mode/effect on this same physical array**, not a separate physical LED
   string — there is no dedicated Shooting Star hardware module. Any zone addressing from the
   original protocol (SS1/SS2/SS3) becomes a **virtual zone** — a defined sub-range of the 16-pixel
   array — rather than a physically separate output. No PWM requirement for this pin, any GPIO
   works for bit-banged/library-driven WS2812 timing.

## Newly available pins

Freed up by eliminating the Main↔Bot-Star and Main↔Top-Star inter-chip links, plus pins that were
already unconnected on Main:

| Pin | Port | Datasheet alt. function | Previously | Hardware PWM? |
|-----|------|--------------------------|------------|----------------|
| 1  | PD3 | OC2B | → Bot-Star pin 24 (link) | Yes (Timer2) |
| 2  | PD4 | T0/XCK | → Bot-Star pin 23 (link) | No |
| 9  | PD5 | OC0B | NC | Yes (Timer0) |
| 10 | PD6 | OC0A | NC | Yes (Timer0) |
| 13 | PB1 | OC1A | NC | Yes (Timer1) |
| 14 | PB2 | OC1B | NC | Yes (Timer1) |
| 26 | PC3 | ADC3 | → Top-Star pin 25 (link) | No |
| 27 | PC4 | ADC4/SDA | → Top-Star pin 24 (link) | No |
| 28 | PC5 | ADC5/SCL | → Top-Star pin 26 (link) | No |
| 25 | PC2 | ADC2 | Untraced (via) — **do not use until confirmed free via continuity check** | No |

## Proposed assignment

| Function | Pin | Port | Peripheral used |
|----------|-----|------|------------------|
| **Ambient — Red** | 9 | PD5 | OC0B hardware PWM |
| **Ambient — Green** | 10 | PD6 | OC0A hardware PWM |
| **Ambient — Blue** | 13 | PB1 | OC1A hardware PWM |
| **Ambient — White** | 14 | PB2 | OC1B hardware PWM |
| **Star array WS2812 data (16 LEDs, Top+Bottom in series)** | 26 | PC3 | Bit-banged digital out |
| *Spare — PWM-capable* | 1 | PD3 | OC2B, reserved (future 5th channel / status LED) |
| *Spare — digital* | 2 | PD4 | Reserved (future use — see note below) |
| *Spare — digital* | 27 | PC4 | Reserved (future use — see note below) |
| *Spare — digital* | 28 | PC5 | Reserved (future use — see note below) |
| *Unresolved — do not use yet* | 25 | PC2 | Confirm free via continuity check first |

Merging Top and Bottom into one series-wired 16-LED array (instead of two separate 8-LED outputs)
frees two additional pins versus the original proposal. Four spares are now available — comfortable
headroom for a genuinely separate physical add-on later (e.g. a real second array, an external
trigger/sensor input) without another re-map, though the current plan needs none of them.

## Unchanged from original design (verify no regressions)

| Function | Pin(s) | Notes |
|----------|--------|-------|
| Wired RS232 software serial | 11 (PD7), 12 (PB0) | → MAX232, preserve exactly for backward compatibility |
| Wireless hardware USART | 30 (PD0/RXD), 31 (PD1/TXD) | → ESP module, preserve exactly |
| ISP-6 programming header | 15, 16, 17, 29 | MOSI/MISO/SCK/RESET — do not repurpose |
| Crystal | 7, 8 | 16MHz |
| AREF decoupling | 20 | 0.1µF cap |
| Pushbutton input | 23 | SW4 |
| Transistor switch | 24 | T1 (8050 NPN) base |
| Status LED | 18 (AVCC-adjacent) | As originally wired |
| Power / ground | 3, 4, 5, 6, 18, 19(analog-only,NC), 21, 22(analog-only,NC), 32 | Unchanged |

## Notes and caveats

- **Pin 12/31 MAX232 conflict is still unresolved** (see hardware notes doc) — needs the
  continuity check between those two pins before finalizing whether the wired-serial TX source is
  hardware USART or bit-banged software serial. This matters for the schematic revision since it
  determines whether pin 31 needs to stay dual-purposed or can be simplified.
- **Timer0 is shared with Arduino core's `millis()`/`micros()` timing** — using OC0A/OC0B for PWM
  is completely standard (this is exactly how Arduino Uno's D5/D6 PWM works) and does not
  conflict. Worth noting: WS2812 bit-banged output libraries (FastLED, Adafruit_NeoPixel) briefly
  disable interrupts during transmission, which can cause minor `millis()` drift — a known
  AVR+WS2812 caveat unrelated to this specific pin choice, not something introduced by this map.
- **ADC6/ADC7 (pins 19, 22) are analog-input-only** on the 32-pin package — no digital I/O
  function exists on these pins at all, confirmed correctly excluded from consideration above.
- Two spare pins (one PWM-capable, one plain digital) are intentionally reserved rather than
  packed tight, to leave room for a revision without another full re-map.
