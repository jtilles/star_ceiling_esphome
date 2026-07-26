"""Sweeps every command in commands.build_sweep() against real hardware,
capturing the resulting LED data-line activity with a Saleae logic analyzer,
synchronized to the actual UART bytes sent. Each capture is decoded (Async
Serial on the UART line, Addressable LEDs (Async) / WS2812B on each LED
line) and exported as a combined decoded data table CSV, not raw timing.

Prerequisites: config.py filled in (see README.md), Logic 2 desktop app
running with the Automation server enabled, analyzer wired to the UART RX
line and both LED data lines, serial adapter connected to the board.

Usage:
    python capture_sweep.py
"""

import concurrent.futures
import json
import sys
import time
import traceback
from datetime import datetime, timezone
from pathlib import Path
from typing import List, Tuple

import serial
from saleae.automation import (
    Capture,
    CaptureConfiguration,
    DigitalTriggerCaptureMode,
    DigitalTriggerType,
    LogicDeviceConfiguration,
    Manager,
)
from saleae.automation.errors import SaleaeError

import config
from commands import SweepCommand, build_sweep

REQUIRED_CONFIG_FIELDS = [
    "SERIAL_PORT",
    "SALEAE_DEVICE_ID",
    "UART_RX_CHANNEL",
    "DIGITAL_SAMPLE_RATE_HZ",
]


def validate_config() -> None:
    missing = []
    for name in REQUIRED_CONFIG_FIELDS:
        value = getattr(config, name)
        if value is None or value == "TODO":
            missing.append(name)
    # TOP_ROW_CHANNEL / BOTTOM_ROW_CHANNEL are only required when that row is
    # actually being captured this run.
    if config.INCLUDE_TOP_ROW and config.TOP_ROW_CHANNEL is None:
        missing.append("TOP_ROW_CHANNEL")
    if config.INCLUDE_BOTTOM_ROW and config.BOTTOM_ROW_CHANNEL is None:
        missing.append("BOTTOM_ROW_CHANNEL")
    if missing:
        print("config.py is not fully filled in. Missing: " + ", ".join(missing), file=sys.stderr)
        print(
            "Run list_saleae_devices.py and a manual Logic 2 capture first -- see README.md.",
            file=sys.stderr,
        )
        sys.exit(1)
    if not config.INCLUDE_TOP_ROW and not config.INCLUDE_BOTTOM_ROW:
        print("Note: INCLUDE_TOP_ROW and INCLUDE_BOTTOM_ROW are both False -- only the UART line will be captured.")


def connect_serial() -> serial.Serial:
    try:
        return serial.Serial(
            port=config.SERIAL_PORT,
            baudrate=config.SERIAL_BAUD_RATE,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1,
        )
    except serial.SerialException as exc:
        print(f"Could not open serial port {config.SERIAL_PORT!r}: {exc}", file=sys.stderr)
        sys.exit(1)


def connect_saleae() -> Manager:
    try:
        manager = Manager.connect(port=config.SALEAE_AUTOMATION_PORT)
    except SaleaeError as exc:
        print(f"Could not connect to Logic 2 automation server: {exc}", file=sys.stderr)
        print(
            "Is Logic 2 running with the Automation server enabled "
            "(Preferences -> scroll to bottom)?",
            file=sys.stderr,
        )
        sys.exit(1)

    device_ids = {d.device_id for d in manager.get_devices(include_simulation_devices=True)}
    if config.SALEAE_DEVICE_ID not in device_ids:
        manager.close()
        print(
            f"SALEAE_DEVICE_ID {config.SALEAE_DEVICE_ID!r} not found among attached devices: "
            f"{sorted(device_ids)}. Run list_saleae_devices.py to check.",
            file=sys.stderr,
        )
        sys.exit(1)

    return manager


def _wait_for_trigger(capture: Capture, timeout_seconds: float) -> None:
    """capture.wait() has no built-in timeout, so it blocks forever if the
    triggering UART falling edge never arrives. Run it on a worker thread and
    force-stop the capture if it doesn't finish in time, so a bad command or
    wiring fault fails that cycle instead of hanging the whole sweep."""
    with concurrent.futures.ThreadPoolExecutor(max_workers=1) as executor:
        future = executor.submit(capture.wait)
        try:
            future.result(timeout=timeout_seconds)
        except concurrent.futures.TimeoutError:
            try:
                capture.stop()
            except SaleaeError:
                pass  # capture may have completed right at the timeout boundary
            raise TimeoutError(
                f"capture did not trigger within {timeout_seconds}s "
                "(no qualifying UART falling edge seen)"
            ) from None


def run_single_capture(
    manager: Manager,
    ser: serial.Serial,
    item: SweepCommand,
    output_root: Path,
) -> None:
    if item.pre_command:
        ser.write(item.pre_command.encode("ascii"))
        ser.flush()
        time.sleep(config.PRE_COMMAND_SETTLE_SECONDS)

    enabled_channels = [config.UART_RX_CHANNEL]
    if config.INCLUDE_TOP_ROW:
        enabled_channels.append(config.TOP_ROW_CHANNEL)
    if config.INCLUDE_BOTTOM_ROW:
        enabled_channels.append(config.BOTTOM_ROW_CHANNEL)

    device_config = LogicDeviceConfiguration(
        enabled_digital_channels=enabled_channels,
        digital_sample_rate=config.DIGITAL_SAMPLE_RATE_HZ,
        digital_threshold_volts=config.DIGITAL_THRESHOLD_VOLTS,
    )

    capture_mode = DigitalTriggerCaptureMode(
        trigger_type=DigitalTriggerType.FALLING,
        trigger_channel_index=config.UART_RX_CHANNEL,
        after_trigger_seconds=config.CAPTURE_AFTER_TRIGGER_SECONDS,
        trim_data_seconds=config.TRIM_DATA_SECONDS,
    )

    capture_config = CaptureConfiguration(capture_mode=capture_mode)

    category_dir = output_root / item.category
    category_dir.mkdir(parents=True, exist_ok=True)

    sent_at = None
    with manager.start_capture(
        device_id=config.SALEAE_DEVICE_ID,
        device_configuration=device_config,
        capture_configuration=capture_config,
    ) as capture:
        # No true pre-trigger buffer exists in the Automation API -- recording
        # starts at arm time, so sleeping here before sending the command is
        # what gives the export its leading idle-line data. See config.py.
        time.sleep(config.PRE_ROLL_SECONDS)

        sent_at = datetime.now(timezone.utc)
        ser.write(item.command.encode("ascii"))
        ser.flush()

        _wait_for_trigger(capture, config.CAPTURE_WAIT_TIMEOUT_SECONDS)

        analyzer_handles = [
            capture.add_analyzer(
                config.UART_ANALYZER_NAME,
                label="uart_rx",
                settings={
                    "Input Channel": config.UART_RX_CHANNEL,
                    "Bit Rate (Bits/s)": config.SERIAL_BAUD_RATE,
                },
            )
        ]
        if config.INCLUDE_TOP_ROW:
            analyzer_handles.append(
                capture.add_analyzer(
                    config.WS2812_ANALYZER_NAME,
                    label="top_row",
                    settings={
                        "LED Channel": config.TOP_ROW_CHANNEL,
                        "LED Controller": config.WS2812_LED_CONTROLLER,
                    },
                )
            )
        if config.INCLUDE_BOTTOM_ROW:
            analyzer_handles.append(
                capture.add_analyzer(
                    config.WS2812_ANALYZER_NAME,
                    label="bottom_row",
                    settings={
                        "LED Channel": config.BOTTOM_ROW_CHANNEL,
                        "LED Controller": config.WS2812_LED_CONTROLLER,
                    },
                )
            )

        dest_csv = category_dir / f"{item.label}.csv"
        capture.export_data_table(str(dest_csv), analyzer_handles)

    sidecar = category_dir / f"{item.label}.json"
    sidecar.write_text(
        json.dumps(
            {
                "category": item.category,
                "label": item.label,
                "command_sent": item.command,
                "pre_command_sent": item.pre_command,
                "notes": item.notes,
                "sent_at_utc": sent_at.isoformat(),
                "pre_roll_seconds": config.PRE_ROLL_SECONDS,
                "after_trigger_seconds": config.CAPTURE_AFTER_TRIGGER_SECONDS,
                "digital_sample_rate_hz": config.DIGITAL_SAMPLE_RATE_HZ,
                "uart_rx_channel": config.UART_RX_CHANNEL,
                "top_row_channel": config.TOP_ROW_CHANNEL if config.INCLUDE_TOP_ROW else None,
                "bottom_row_channel": config.BOTTOM_ROW_CHANNEL if config.INCLUDE_BOTTOM_ROW else None,
                "ws2812_led_controller": config.WS2812_LED_CONTROLLER,
            },
            indent=2,
        ),
        encoding="utf-8",
    )


def main() -> int:
    validate_config()

    ser = connect_serial()
    manager = connect_saleae()

    # export_raw_data_csv's directory argument is resolved by the Logic 2
    # desktop process, not this script -- a relative path gets interpreted
    # against Logic 2's own working directory (often somewhere unwritable,
    # e.g. under Program Files), not this repo. Must be absolute.
    output_root = Path(config.OUTPUT_DIR).resolve()
    output_root.mkdir(parents=True, exist_ok=True)

    sweep = build_sweep(
        include_shooting_star=config.INCLUDE_SHOOTING_STAR_SWEEP,
        include_ambient=config.INCLUDE_AMBIENT_SWEEP,
    )
    if not config.INCLUDE_SHOOTING_STAR_SWEEP:
        print("Skipping shooting_star sweep (config.INCLUDE_SHOOTING_STAR_SWEEP=False)")
    if not config.INCLUDE_AMBIENT_SWEEP:
        print("Skipping ambient_effects/ambient_colors/strip_dimmer sweep (config.INCLUDE_AMBIENT_SWEEP=False)")

    succeeded: List[str] = []
    failed: List[Tuple[str, str]] = []

    try:
        for i, item in enumerate(sweep, start=1):
            label = f"{item.category}/{item.label}"
            print(f"[{i}/{len(sweep)}] {label}  (command={item.command!r})")
            try:
                run_single_capture(manager, ser, item, output_root)
                succeeded.append(label)
            except Exception as exc:
                print(f"    FAILED: {exc}", file=sys.stderr)
                traceback.print_exc(file=sys.stderr)
                failed.append((label, str(exc)))
            time.sleep(config.INTER_COMMAND_DELAY_SECONDS)
    finally:
        ser.close()
        manager.close()

    print("\n--- Sweep summary ---")
    print(f"Succeeded: {len(succeeded)}/{len(sweep)}")
    if failed:
        print(f"Failed: {len(failed)}")
        for label, error in failed:
            print(f"  {label}: {error}")

    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
