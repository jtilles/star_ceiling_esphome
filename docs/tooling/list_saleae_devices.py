"""Run this first, before filling in config.py, to find your Saleae device_id.

Prerequisite: Logic 2 desktop app running, with a device plugged in, and the
Automation server enabled (Preferences -> scroll to bottom -> enable).

Usage:
    python list_saleae_devices.py
"""

import sys

from saleae.automation import Manager
from saleae.automation.errors import SaleaeError

import config


def main() -> int:
    try:
        with Manager.connect(port=config.SALEAE_AUTOMATION_PORT) as manager:
            devices = manager.get_devices(include_simulation_devices=True)
    except SaleaeError as exc:
        print(f"Could not talk to Logic 2: {exc}", file=sys.stderr)
        print(
            "Is Logic 2 running with the Automation server enabled "
            "(Preferences -> scroll to bottom)?",
            file=sys.stderr,
        )
        return 1

    if not devices:
        print("No devices found. Is a Logic analyzer plugged in and detected by Logic 2?")
        return 1

    print("Found devices:")
    for device in devices:
        print(
            f"  device_id={device.device_id!r}  "
            f"type={device.device_type.name}  "
            f"simulation={device.is_simulation}"
        )
    print("\nCopy the device_id of your real (non-simulation) device into config.SALEAE_DEVICE_ID.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
