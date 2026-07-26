# Illuminate Firmware Notes (Ghidra Reverse-Engineering)

Findings from Ghidra-decompiled C dumped off the physical chips: `binaries/original_dumps/TOP-STAR.bin.c`, `BOT-STAR.bin.c`, and `RGBW.bin.c`. This is disassembly-derived evidence, layered on top of the manual-derived protocol already implemented in `components/illuminate/illuminate_protocol.cpp`.

## Firmware images

| Chip     | File               | Size (bytes) | Lines | Notes |
|----------|--------------------|--------------|-------|-------|
| TOP-STAR | `TOP-STAR.bin.c`   | 193,671      | 6,252 | Identical to BOT-STAR (0 diff) |
| BOT-STAR | `BOT-STAR.bin.c`   | 193,671      | 6,252 | Identical to TOP-STAR (0 diff) |
| RGBW     | `RGBW.bin.c`       | 83,258       | 3,270 | Distinct, much simpler firmware |
| MAIN     | `MAIN/main.bin.c`  | ~26,000      | —     | Not analyzed in this pass |

**TOP-STAR and BOT-STAR run the exact same firmware image** — `diff` between the two decompiled files returns zero differences. Any behavioral difference between the two boards comes from external wiring/strapping, not firmware.

All three analyzed images are standard **Arduino/AVR-GCC builds** on what's being treated as an ATmega328P (matches the datasheet in `docs/` and the `avrdude -p m328p` dump script), not custom bare-metal firmware. Ghidra recognized the full AVR interrupt vector table and several Arduino-core library functions:
- `FUN_code_0006b5` (TOP/BOT-STAR) — matches Arduino core's `init()` (Timer0 fast-PWM, Timer1/2 PWM setup, ADC enable) almost exactly, with the sketch's `setup()`+`loop()` inlined right after — this function is effectively the whole runtime, ending in its own internal infinite loop.
- `TIMER0_OVF` — canonical Arduino `millis()` overflow handler (matching magic constants 3 / 0x7d / 0x86 fractional compensation).
- `USART_RX` / `USART_UDRE` — standard `HardwareSerial` 64-byte ring-buffer ISRs (`& 0x3f` mask confirms buffer size).
- Nearly every other interrupt vector (SPI, ADC-complete, analog comparator, TWI, watchdog, pin-change, INT0/1, etc.) is unused and decompiles to an identical reset-like stub — Ghidra just separately decompiled each unused vector's shared jump target.

No string literals and no C++ vtables exist in any of the files (size-optimized build, no debug info).

## RXD command handler (pin 30 / PD0) — TOP-STAR & BOT-STAR

The ESP/Bluetooth module's TX line lands on PD0 (RXD) on all chips. Confirmed handler chain:

1. `USART_RX` ISR pushes each received byte into the standard Arduino Serial RX ring buffer.
2. `FUN_code_0001b7` / `FUN_code_0001cb` / `FUN_code_0001d9` are `Serial.read()` / `peek()` / `available()` respectively (recognizable from the `HardwareSerial` object's field offsets: `_rx_buffer_head`/`_tail` at +0x19/+0x1a).
3. The main loop (`FUN_code_0006b5`) polls `Serial.available()` on every animation tick. When a byte is pending, it breaks out of the per-star animation loop into a large if/else decision tree (compiled from what was originally a `switch` on the received character) starting around **line 4580** of `TOP-STAR.bin.c`.
4. That dispatcher reads one command byte with `Serial.read()` and branches on exact ASCII values.

### Command table — TOP-STAR / BOT-STAR (Starry Night side)

| Byte | Char | Confirmed meaning | Matches `illuminate_protocol.cpp` |
|------|------|--------------------|-------------------------------------|
| 0x54 | `T`  | Twinkle effect select | `StarryEffect::TWINKLE` |
| 0x46 | `F`  | Firefly effect select | `StarryEffect::FIREFLY` |
| 0x42 | `B`  | Breeze effect select | `StarryEffect::BREEZE` |
| 0x52 | `R`  | Random effect select | `StarryEffect::RANDOM` |
| 0x50 | `P`  | Rain Drops effect select | `StarryEffect::RAIN_DROPS` |
| 0x4a | `J`  | Jamboree effect select | `StarryEffect::JAMBOREE` |
| 0x77 | `w`  | Wipe color transition | `ColorTransition::WIPE` |
| 0x66 | `f`  | Fade color transition | `ColorTransition::FADE` |
| 0x72 | `r`  | Rainbow color transition | `ColorTransition::RAINBOW` |
| 0x62 | `b`  | Blink color transition | `ColorTransition::BLINK` |
| 0x63 | `c`  | Set Starry color: reads 4 comma-delimited numeric fields (R,G,B,W) then a `*` terminator | `starry_color_command` |
| 0x44 | `D`  | Star dimmer: reads one numeric field (percent), scales the stored RGBW color by it | `star_dimmer_command` |
| **0x4f** | **`O`** | **Undocumented** — handled identically in structure to the other effect-select commands (clears all other effect timers, sets its own independent state pair), but there is no `O` case anywhere in `illuminate_protocol.cpp`/the PRD. This is a **7th Starry-effect slot** the firmware supports that isn't implemented or named anywhere yet. | *(none — new finding)* |

Every command byte the firmware's decision tree checks for is accounted for above — the full set matches the PRD's statement that "original protocol commands are single uppercase/lowercase letters, and comma-delimited strings" (`docs/illuminate-enhanced-prd.md:153`), plus the one addition.

### Shooting Star is absent from this firmware (correction + confirmation)

An earlier pass over this same code misread the block around line 4318–4545 of `TOP-STAR.bin.c` as an "8-slot shooting-star allocator." Closer reading shows it's actually the **Fade/Blink color-transition easing logic**: `DAT_mem_020b`/`020c`/`020d` are the current R/G/B channel values (set via `FUN_code_000564`/`00056b`/`00055d`, each a trivial `set channel N = value` — confirmed by reading their bodies directly), and the surrounding chain of comparisons steps each channel one increment at a time toward a target (clamped against `0xff`/`0`). That's smooth per-channel ramping for the `'f'` (Fade) and `'b'` (Blink) commands, not shooting-star trajectory rendering. The chained `DAT_mem_01bd..01ce` state pairs are concurrency slots for that ramp, not shooting-star instances.

**Confirmed: TOP-STAR/BOT-STAR's command dispatcher has no shooting-star support at all.** `illuminate_protocol.cpp`'s shooting-star command uses zone characters `X`/`Y`/`Z` (`zone_char()` in `illuminate_protocol.cpp`), so those were grepped for directly against the full command-parsing region of `TOP-STAR.bin.c` (both hex `0x58/0x59/0x5a` and decimal forms) — **zero matches**. There is no gating flag, no hardware-strap check, and no dormant code path: the decision tree simply never branches on those bytes, so sending `X`/`Y`/`Z` to this firmware does nothing — the bytes are read and silently dropped.

This is firmware-image-level absence, not a runtime-disableable feature. It **confirms** (rather than casts doubt on) the PRD's SP vs. SS firmware-split assumption: this specific dump is genuinely SP (no shooting star) firmware. Getting shooting star working would require dumping and decompiling an actual SS-firmware unit — a different binary — not toggling anything in this one.

### LED output mechanism — flash-resident jump table

`FUN_code_001806` performs an **indirect call through a function-pointer table in flash**, indexed by `position*2`. Ghidra couldn't resolve it as an ordinary jump table ("too many branches"), consistent with roughly 93 (0x5d) distinct entries. Combined with the already-confirmed WS2812-style single-wire protocol on PD6 (`docs/illuminate-hardware-notes.md`), this points to **one hand-unrolled, cycle-exact flash routine per LED/star position** — a common technique for driving many WS2812-like pixels via bit-banging when timing margins are too tight for a shared loop.

### Animation engine

The main loop initializes and steps an array of small per-star state structs (repeated calls to a helper that zero out sequential `DAT_mem_01xx`/`02xx` variable pairs at startup — one pair per star, holding fade/target/timer state). The ADC is enabled and read (`ADMUX = 0x40`) early in the main loop, most likely as a `randomSeed()`-style entropy source for twinkle randomization.

## RGBW.bin.c — ambient light controller

RGBW.bin.c is a much smaller, simpler firmware (83,258 bytes vs. 193,671 for the star chips) — consistent with driving a single PWM-dimmed 4-channel ambient light rather than dozens of individually addressable pixels.

- Same recognizable Arduino/AVR-GCC core (interrupt vector table, `HardwareSerial`, `millis()`).
- Uses **hardware PWM** — all six timer output-compare registers are referenced (`OCR0A`, `OCR0B`, `OCR1A`, `OCR1B`, `OCR2A`, `OCR2B`) — rather than the WS2812 bit-bang jump table used by the star chips. This confirms the RGBW board dims its 4 channels (R, G, B, W) via straightforward hardware PWM, not an addressable-pixel protocol.
- Has its own serial command dispatcher (same `Serial.available()`/`read()` pattern as the star chips), starting around line 2570 of `RGBW.bin.c`.

### Command table — RGBW (Ambient side)

| Byte | Char | Confirmed meaning | Matches `illuminate_protocol.cpp` |
|------|------|--------------------|-------------------------------------|
| 0x65 | `e`  | Fade ambient effect | `AmbientEffect::FADE` |
| 0x6b | `k`  | Blink ambient effect | `AmbientEffect::BLINK` |
| 0x6e | `n`  | No effect / static color | `AmbientEffect::NO_EFFECT` |
| 0x41 | `A`  | Set ambient color: reads 4 comma-delimited numeric fields (R,G,B,W) then a `*` terminator | `ambient_color_command` |
| 0x64 | `d`  | Ambient dimmer: reads one numeric field (percent), scales the stored RGBW color by it | `ambient_dimmer_command` |

This is an **exact match** to the ambient-side functions in `illuminate_protocol.cpp` — no discrepancies or extra commands found in the portion reviewed.

## Open questions / suggested next steps

1. **Bench-test the `O` command** on a TOP-STAR or BOT-STAR unit (send raw `O` over the RS232 line, observe the WS2812 line with the logic analyzer) to identify the 7th Starry effect and decide whether to add it to `illuminate_protocol.cpp`/the light platform.
2. **Shooting Star needs a real SS-firmware dump.** This TOP-STAR/BOT-STAR image has no `X`/`Y`/`Z` handling whatsoever — it can't be enabled via command, fuse, or strap on this binary. To implement Shooting Star, an actual SS-firmware unit still needs to be sourced, dumped, and decompiled the same way.
3. If useful later, correlate the ~93-entry WS2812 jump table against the pinmap/timing captures in `captures/` to confirm per-star timing and pixel ordering.
4. MAIN.bin.c hasn't been analyzed with this level of detail yet — it likely owns the top-level RS232-to-per-chip routing (or simply passes bytes through to all three chips on a shared bus, since each chip already parses its own subset of the same command grammar directly).
