"""Shared parsing helpers for Saleae raw digital.csv exports (used by
analyze_raw_timing.py and decode_ws2812_rgbw.py). No Saleae/serial deps --
pure CSV/timing logic so it's testable against synthetic data.
"""

import csv
import re


def load_channel_transitions(csv_path: str, channel: int):
    """Returns a list of (time_seconds, state) for every row in the raw
    export, read from whichever column matches the given channel number."""
    with open(csv_path, newline="") as f:
        reader = csv.reader(f)
        header = next(reader)

        time_col = None
        for candidate in ("Time [s]", "Trigger [s]"):
            if candidate in header:
                time_col = header.index(candidate)
                break
        if time_col is None:
            raise ValueError(f"Couldn't find a time column in header: {header}")

        channel_col = None
        for i, name in enumerate(header):
            match = re.search(r"(\d+)\s*$", name)
            if match and int(match.group(1)) == channel and i != time_col:
                channel_col = i
                break
        if channel_col is None:
            raise ValueError(
                f"Couldn't find column for channel {channel} in header: {header}. "
                "Pass the channel number explicitly if auto-detection is wrong."
            )

        print(f"Header: {header}")
        print(f"Using time column {time_col!r}, channel {channel} -> column {header[channel_col]!r}\n")

        rows = [(float(row[time_col]), int(row[channel_col])) for row in reader]
    return rows


def pulses_from_transitions(rows):
    """Collapses a (time, state) sample stream (which repeats the same state
    across rows where OTHER channels changed) into (state, duration_seconds)
    pulses -- one per contiguous run of a single state."""
    pulses = []
    run_state = None
    run_start = None
    for t, state in rows:
        if run_state is None:
            run_state, run_start = state, t
        elif state != run_state:
            pulses.append((run_state, t - run_start))
            run_state, run_start = state, t
    return pulses
