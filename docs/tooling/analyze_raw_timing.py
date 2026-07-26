"""Analyzes a raw digital.csv from diagnose_ws2812_timing.py: measures actual
bit-level pulse widths on one channel and reports enough to tell (a) which
known WS2812-family chip timing it matches, and (b) how many bits are sent
per LED-refresh frame -- the key number for telling RGB (24 bits/LED) apart
from RGBW (32 bits/LED) framing, which is what's needed to explain why the
built-in Saleae "Addressable LEDs (Async)" analyzer (RGB-only, no RGBW mode)
shows solid-color commands as a rotating 4-way pattern instead of uniform.

Usage:
    python analyze_raw_timing.py path\\to\\digital.csv [channel_number]

    channel_number defaults to config.TOP_ROW_CHANNEL if omitted.
"""

import sys
from collections import Counter

import config
from raw_csv_utils import load_channel_transitions, pulses_from_transitions

# Reference timing (nanoseconds), transcribed from Saleae's own analyzer
# source (AsyncRgbLedAnalyzerSettings.cpp -- InitControllerData()):
# https://github.com/saleae/async-rgb-led-analyzer
# Each entry: (0-bit high ns, 1-bit high ns) as (min, mid, max) tuples, plus
# nominal total bit period for reference. All of these are 3-channel
# (RGB, 24 bits/LED) *except that none of them support a 4th (W) channel* --
# that's the actual limitation under investigation, not a per-chip timing
# detail.
REFERENCE_TIMING_NS = {
    "WS2811 (400kHz)": {"0_high": (350, 500, 650), "1_high": (1050, 1200, 1350), "period": 2500},
    "WS2811 (800kHz)": {"0_high": (175, 250, 325), "1_high": (525, 600, 675), "period": 1250},
    "WS2812B": {"0_high": (200, 400, 550), "1_high": (650, 800, 1050), "period": 1250},
    "WS2813": {"0_high": (300, 375, 450), "1_high": (750, 875, 1000), "period": 1250},
    "TM1809": {"0_high": (450, 600, 750), "1_high": (1050, 1200, 1350), "period": 1650},
    "UCS1903": {"0_high": (350, 500, 650), "1_high": (1850, 2000, 2150), "period": 2500},
}

RESET_THRESHOLD_NS = 5000  # low pulses longer than this are frame boundaries


def main() -> int:
    if len(sys.argv) < 2:
        print("Usage: python analyze_raw_timing.py path\\to\\digital.csv [channel_number]", file=sys.stderr)
        return 1

    csv_path = sys.argv[1]
    channel = int(sys.argv[2]) if len(sys.argv) > 2 else config.TOP_ROW_CHANNEL

    rows = load_channel_transitions(csv_path, channel)
    pulses = pulses_from_transitions(rows)

    high_pulses_ns = [d * 1e9 for state, d in pulses if state == 1]
    low_pulses_ns = [d * 1e9 for state, d in pulses if state == 0]

    if not high_pulses_ns:
        print("No high pulses found on that channel -- wrong channel number, or line was idle the whole capture.")
        return 1

    # Split high pulses into "0-bit" (short) vs "1-bit" (long) clusters using
    # the midpoint between min and max as a simple threshold.
    lo, hi = min(high_pulses_ns), max(high_pulses_ns)
    split = (lo + hi) / 2
    zero_bit_highs = [v for v in high_pulses_ns if v <= split]
    one_bit_highs = [v for v in high_pulses_ns if v > split]

    def avg(vals):
        return sum(vals) / len(vals) if vals else float("nan")

    print(f"High pulses: {len(high_pulses_ns)} total, range {lo:.0f}-{hi:.0f} ns")
    print(f"  0-bit cluster: {len(zero_bit_highs)} pulses, avg {avg(zero_bit_highs):.0f} ns")
    print(f"  1-bit cluster: {len(one_bit_highs)} pulses, avg {avg(one_bit_highs):.0f} ns")

    print("\nClosest known chip timing (compare avg 0-bit/1-bit high times above):")
    for name, spec in REFERENCE_TIMING_NS.items():
        z_lo, z_mid, z_hi = spec["0_high"]
        o_lo, o_mid, o_hi = spec["1_high"]
        print(f"  {name:20s} 0-bit high {z_lo}-{z_hi} ns (mid {z_mid}), 1-bit high {o_lo}-{o_hi} ns (mid {o_mid})")

    # Frame boundaries: long low pulses (reset/latch gaps).
    reset_indices = [i for i, (state, d) in enumerate(pulses) if state == 0 and d * 1e9 > RESET_THRESHOLD_NS]
    print(f"\nFound {len(reset_indices)} reset/latch gaps (low pulse > {RESET_THRESHOLD_NS} ns).")

    bits_per_frame = []
    prev_reset = -1
    for idx in reset_indices:
        frame_pulses = pulses[prev_reset + 1 : idx]
        n_bits = sum(1 for state, _ in frame_pulses if state == 1)
        if n_bits > 0:
            bits_per_frame.append(n_bits)
        prev_reset = idx

    if bits_per_frame:
        counts = Counter(bits_per_frame)
        print(f"\nBits per frame (between resets): {sorted(counts.items())}")
        most_common_bits, _ = counts.most_common(1)[0]
        print(f"Most common frame size: {most_common_bits} bits")
        print(f"  As RGB  (24 bits/LED): {most_common_bits / 24:.2f} LEDs/frame")
        print(f"  As RGBW (32 bits/LED): {most_common_bits / 32:.2f} LEDs/frame")
        print(
            "\nIf the RGBW line divides evenly (near a whole number) and the RGB line "
            "doesn't, that confirms the RGBW-misdecoded-as-RGB theory."
        )
    else:
        print("Not enough reset gaps found to measure frame size -- try a longer capture.")

    return 0


if __name__ == "__main__":
    sys.exit(main())
