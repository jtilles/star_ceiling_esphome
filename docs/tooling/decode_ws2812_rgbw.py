"""Decodes a raw digital.csv as 32-bit-per-LED (RGBW) WS2812-style frames --
the framing the built-in Saleae "Addressable LEDs (Async)" analyzer can't
represent (it's hardcoded to 24-bit RGB). Confirmed via analyze_raw_timing.py
on a real capture: 256 bits/frame, which is exactly 8 LEDs at 32 bits/LED
(24 bits/LED gives a non-integer 10.67).

Byte order within each 32-bit LED frame is NOT yet known -- WS2812B itself
is GRB, but that says nothing about a 4th, vendor-defined W byte's position.
This script prints each frame's 4 raw bytes per LED, unlabeled, so they can
be matched against a known commanded color (e.g. solid Red, c,255,0,0,0*)
to figure out which byte position is R/G/B/W by inspection.

Usage:
    python decode_ws2812_rgbw.py path\\to\\digital.csv [channel_number] [max_frames]
"""

import sys

import config
from raw_csv_utils import load_channel_transitions, pulses_from_transitions

RESET_THRESHOLD_NS = 5000


def bits_to_frames(pulses, bit_threshold_ns):
    """Splits a pulse list into frames (lists of 0/1 bits), delimited by
    long low pulses (resets). Only HIGH pulses carry bit data; a pulse's
    duration (not its low-side partner) is what encodes 0 vs 1 for these
    single-wire LED protocols."""
    frames = []
    current = []
    for state, duration in pulses:
        if state == 1:
            current.append(1 if duration * 1e9 > bit_threshold_ns else 0)
        else:
            if duration * 1e9 > RESET_THRESHOLD_NS:
                if current:
                    frames.append(current)
                    current = []
    if current:
        frames.append(current)
    return frames


def bits_to_bytes(bits):
    bytes_out = []
    for i in range(0, len(bits) - 7, 8):
        byte = 0
        for b in bits[i : i + 8]:
            byte = (byte << 1) | b
        bytes_out.append(byte)
    return bytes_out


def main() -> int:
    if len(sys.argv) < 2:
        print("Usage: python decode_ws2812_rgbw.py path\\to\\digital.csv [channel_number] [max_frames]", file=sys.stderr)
        return 1

    csv_path = sys.argv[1]
    channel = int(sys.argv[2]) if len(sys.argv) > 2 else config.TOP_ROW_CHANNEL
    max_frames = int(sys.argv[3]) if len(sys.argv) > 3 else 5

    rows = load_channel_transitions(csv_path, channel)
    pulses = pulses_from_transitions(rows)

    high_ns = [d * 1e9 for state, d in pulses if state == 1]
    if not high_ns:
        print("No high pulses found on that channel.")
        return 1
    bit_threshold_ns = (min(high_ns) + max(high_ns)) / 2

    frames = bits_to_frames(pulses, bit_threshold_ns)
    print(f"Found {len(frames)} frames. Showing first {max_frames}.\n")

    for frame_idx, bits in enumerate(frames[:max_frames]):
        byte_vals = bits_to_bytes(bits)
        n_leds = len(byte_vals) // 4
        print(f"Frame {frame_idx}: {len(bits)} bits, {len(byte_vals)} bytes, {n_leds} LEDs (4 bytes/LED)")
        for led_idx in range(n_leds):
            b0, b1, b2, b3 = byte_vals[led_idx * 4 : led_idx * 4 + 4]
            print(f"  LED {led_idx}: byte0={b0:3d}  byte1={b1:3d}  byte2={b2:3d}  byte3={b3:3d}")
        leftover_bits = len(bits) - n_leds * 32
        if leftover_bits:
            print(f"  ({leftover_bits} leftover bits not forming a full LED -- unexpected, check bit_threshold)")
        print()

    return 0


if __name__ == "__main__":
    sys.exit(main())
