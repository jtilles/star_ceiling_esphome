"""Sweep command list, built from the confirmed Illuminate RS232 protocol tables.

This module holds only data (which commands to send and how to label their
captures) -- no serial or Saleae API calls live here. See capture_sweep.py
for the capture/serial logic that consumes SweepCommand entries.
"""

from dataclasses import dataclass
from typing import List, Optional


@dataclass(frozen=True)
class SweepCommand:
    category: str  # output subfolder, e.g. "starry_colors"
    label: str  # output file base name, e.g. "color_warm_white"
    command: str  # exact bytes to send and trigger the capture on
    pre_command: Optional[str] = None  # sent (uncaptured) before arming, if set
    notes: str = ""


# name -> (R, G, B, W), shared by Starry Night ("c,") and Ambient ("A,") colors
NAMED_PALETTE = {
    "red": (255, 0, 0, 0),
    "orange": (255, 50, 0, 0),
    "amber": (255, 150, 0, 0),
    "yellow": (255, 255, 0, 0),
    "green": (0, 255, 0, 0),
    "light_green": (0, 255, 150, 0),
    "lime": (0, 255, 50, 0),
    "aqua": (0, 255, 255, 0),
    "blue": (0, 0, 255, 0),
    "violet": (150, 0, 255, 0),
    "purple": (255, 0, 200, 0),
    "pink": (255, 0, 50, 0),
    "white": (0, 0, 0, 255),
    "warm_white": (202, 218, 255, 0),
    "natural_white": (255, 246, 237, 0),
    "cool_white": (255, 193, 141, 0),
}

STARRY_EFFECTS = {
    "twinkle": "T",
    "firefly": "F",
    "breeze": "B",
    "random": "R",
    "rain_drops": "P",
    "jamboree": "J",
}

# Color transitions are modifiers on top of an active color, not a standalone
# mode -- a transition command alone may not produce visible LED activity.
# Each is preceded by an uncaptured priming color command (Red) so the sweep
# actually exercises the transition, matching the "use your judgement" note
# in the source spec.
STARRY_TRANSITIONS = {
    "wipe": "w",
    "fade": "f",
    "rainbow": "r",
    "blink": "b",
}
STARRY_TRANSITION_PRE_COMMAND = "c,255,0,0,0*"  # Red

STAR_DIMMER_PERCENTAGES = [10, 50, 100]
STRIP_DIMMER_PERCENTAGES = [10, 50, 100]

SHOOTING_STAR_ZONES = {
    "ss1": "X",
    "ss2": "Y",
    "ss3": "Z",
}
SHOOTING_STAR_RED = NAMED_PALETTE["red"]

# speed range is 150 (slow) to 255 (fast); sweep both ends
AMBIENT_EFFECT_SPEEDS = [150, 255]


def _rgbw_command(prefix: str, rgbw) -> str:
    r, g, b, w = rgbw
    return f"{prefix},{r},{g},{b},{w}*"


def _rgbw_command_with_time(zone: str, rgbw, minutes: int) -> str:
    r, g, b, w = rgbw
    return f"{zone},{r},{g},{b},{w},{minutes}*"


def build_sweep(include_shooting_star: bool = True, include_ambient: bool = True) -> List[SweepCommand]:
    sweep: List[SweepCommand] = []

    for label, code in STARRY_EFFECTS.items():
        sweep.append(SweepCommand("starry_effects", label, code))

    for label, code in STARRY_TRANSITIONS.items():
        sweep.append(
            SweepCommand(
                "starry_transitions",
                label,
                code,
                pre_command=STARRY_TRANSITION_PRE_COMMAND,
                notes="preceded by an uncaptured Red color command so the transition has an active color to act on",
            )
        )

    for name, rgbw in NAMED_PALETTE.items():
        sweep.append(
            SweepCommand("starry_colors", f"color_{name}", _rgbw_command("c", rgbw))
        )

    for pct in STAR_DIMMER_PERCENTAGES:
        sweep.append(SweepCommand("star_dimmer", f"dimmer_{pct}", f"D,{pct}*"))

    if include_shooting_star:
        for label, zone in SHOOTING_STAR_ZONES.items():
            sweep.append(
                SweepCommand(
                    "shooting_star",
                    f"{label}_red_1min",
                    _rgbw_command_with_time(zone, SHOOTING_STAR_RED, 1),
                )
            )
            sweep.append(
                SweepCommand(
                    "shooting_star",
                    f"{label}_off",
                    _rgbw_command_with_time(zone, (0, 0, 0, 0), 0),
                )
            )

    if include_ambient:
        for speed in AMBIENT_EFFECT_SPEEDS:
            sweep.append(SweepCommand("ambient_effects", f"fade_speed_{speed}", f"e,{speed}"))
            sweep.append(SweepCommand("ambient_effects", f"blink_speed_{speed}", f"k,{speed}"))
        sweep.append(SweepCommand("ambient_effects", "no_effect", "n"))

        for name, rgbw in NAMED_PALETTE.items():
            sweep.append(
                SweepCommand("ambient_colors", f"color_{name}", _rgbw_command("A", rgbw))
            )

        for pct in STRIP_DIMMER_PERCENTAGES:
            sweep.append(SweepCommand("strip_dimmer", f"dimmer_{pct}", f"d,{pct}*"))

    return sweep
