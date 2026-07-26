"""One-off diagnostic capture: sends a single command and exports RAW digital
transition timing (not the analyzer-decoded pixel table), so the actual
bit-level pulse widths can be measured directly.

Why: the decoded 'Addressable LEDs (Async)' / WS2812B output from the main
sweep shows solid-color commands (e.g. c,255,0,0,0*) as a rotating 4-way
R/G/B/black pattern instead of a uniform color, even though the physical
ceiling really does go solid red. Leading theory: the real LEDs are RGBW
(32 bits/LED -- consistent with the RS232 protocol's own R,G,B,W command
shape), but Saleae's built-in analyzer only supports RGB (24 bits/LED, no
RGBW mode -- confirmed via https://discuss.saleae.com/t/analyzer-addressable-leds-configurable-for-rgbw/2604).
A 24-bit decode of a 32-bit stream re-aligns every 4 pixels (lcm(24,32)/24),
which matches the observed period-4 rotation. This script grabs raw timing
so that theory can be checked against the real pulse widths instead of
guessed at.

Usage:
    python diagnose_ws2812_timing.py ["command to send"]

    Defaults to c,255,0,0,0* (solid Starry Night Red) if no argument given.
"""

import sys
import time
from pathlib import Path

from saleae.automation import (
    CaptureConfiguration,
    DigitalTriggerCaptureMode,
    DigitalTriggerType,
    LogicDeviceConfiguration,
)

import config
from capture_sweep import connect_saleae, connect_serial, _wait_for_trigger


def main() -> int:
    command = sys.argv[1] if len(sys.argv) > 1 else "c,255,0,0,0*"

    ser = connect_serial()
    manager = connect_saleae()

    output_dir = Path(config.OUTPUT_DIR).resolve() / "_raw_diagnostic"
    output_dir.mkdir(parents=True, exist_ok=True)

    device_config = LogicDeviceConfiguration(
        enabled_digital_channels=[
            config.UART_RX_CHANNEL,
            config.TOP_ROW_CHANNEL,
            config.BOTTOM_ROW_CHANNEL,
        ],
        digital_sample_rate=config.DIGITAL_SAMPLE_RATE_HZ,
        digital_threshold_volts=config.DIGITAL_THRESHOLD_VOLTS,
    )

    # Short after_trigger window -- we only need a couple of refresh frames'
    # worth of bits to measure pulse widths, not a full 5s capture.
    capture_mode = DigitalTriggerCaptureMode(
        trigger_type=DigitalTriggerType.FALLING,
        trigger_channel_index=config.UART_RX_CHANNEL,
        after_trigger_seconds=1.0,
        trim_data_seconds=1.5,
    )
    capture_config = CaptureConfiguration(capture_mode=capture_mode)

    print(f"Sending {command!r} and capturing raw digital timing...")
    try:
        with manager.start_capture(
            device_id=config.SALEAE_DEVICE_ID,
            device_configuration=device_config,
            capture_configuration=capture_config,
        ) as capture:
            time.sleep(config.PRE_ROLL_SECONDS)
            ser.write(command.encode("ascii"))
            ser.flush()

            _wait_for_trigger(capture, config.CAPTURE_WAIT_TIMEOUT_SECONDS)

            capture.export_raw_data_csv(
                directory=str(output_dir),
                digital_channels=[
                    config.UART_RX_CHANNEL,
                    config.TOP_ROW_CHANNEL,
                    config.BOTTOM_ROW_CHANNEL,
                ],
            )
    finally:
        ser.close()
        manager.close()

    csv_path = output_dir / "digital.csv"
    print(f"Raw digital CSV written to {csv_path}")
    print(f"Now run: python analyze_raw_timing.py \"{csv_path}\"")
    return 0


if __name__ == "__main__":
    sys.exit(main())
