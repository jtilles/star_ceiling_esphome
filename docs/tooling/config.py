"""Configuration for the Illuminate protocol Saleae capture sweep.

Fill in every TODO below before running capture_sweep.py. Run
list_saleae_devices.py first to get SALEAE_DEVICE_ID, and do one manual
capture in the Logic 2 desktop UI to determine the channel numbers and a
safe digital sample rate (see docs/tooling/README.md).
"""

# --- Serial (command) connection --------------------------------------------
# USB-to-TTL serial adapter wired to the board's UART header (5V logic,
# confirmed 9600 8N1, no flow control, against real hardware).
SERIAL_PORT = "COM7"  # e.g. "COM5" on Windows, "/dev/ttyUSB0" on Linux/Mac
SERIAL_BAUD_RATE = 9600

# --- Saleae Logic 2 automation connection -----------------------------------
# Enable the automation server in Logic 2: Preferences -> scroll to bottom.
SALEAE_AUTOMATION_PORT = 10430  # Logic 2 default automation server port
SALEAE_DEVICE_ID = "5BA450C72E4C3070"  # run list_saleae_devices.py to find this

# --- Digital channel assignment ----------------------------------------------
# TODO: these are Saleae *channel numbers* (as wired to your analyzer inputs),
# not list indices. Determine them from your manual Logic 2 capture.
# TOP_ROW_CHANNEL / BOTTOM_ROW_CHANNEL are only required if the matching
# INCLUDE_TOP_ROW / INCLUDE_BOTTOM_ROW flag below is True.
UART_RX_CHANNEL = 2  # trigger channel: UART RX line into the board
TOP_ROW_CHANNEL = 0  # P-TOP zone LED data line
BOTTOM_ROW_CHANNEL = 1  # P-BOTTOM zone LED data line

# --- Sample rate ---------------------------------------------------------------
# TODO: WS2812-style single-wire timing needs a high sample rate (order of tens
# of MHz) to resolve bit timing (~1.25us/bit, ~400ns sub-pulses) accurately. The
# valid sample rates depend on your specific analyzer model and how many
# channels are enabled -- read them off the Device Settings panel during your
# manual capture in Logic 2 and set the value here. Do NOT guess: too low a
# rate will silently produce garbage/aliased timing data that looks plausible.
DIGITAL_SAMPLE_RATE_HZ = 10000000  # e.g. 10000000 for 10 MHz, 20000000 for 20 MHz, etc.

# --- Digital input threshold --------------------------------------------------
# Per the logic2-automation source (LogicDeviceConfiguration.digital_threshold_volts):
# only meaningful for Logic Pro 8 / Logic Pro 16, valid values are exactly
# 1.2, 1.8, or 3.3 -- NOT continuously variable. Leave this as None for a
# standard Logic 8 (fixed input threshold, not configurable via this API).
# The board runs 5V logic; Logic 8's fixed ~1.65V threshold reads a 5V HIGH
# correctly, but confirm this against your specific analyzer's input spec.
DIGITAL_THRESHOLD_VOLTS = None

# --- Capture timing ------------------------------------------------------------
# There is no dedicated "pre-trigger buffer" parameter in the Automation API
# (DigitalTriggerCaptureMode only exposes after_trigger_seconds, which is
# POST-trigger). The workaround used by capture_sweep.py: arm the capture,
# sleep PRE_ROLL_SECONDS, THEN send the command that produces the triggering
# UART falling edge. Recording begins the moment the capture is armed, so the
# exported file naturally includes that sleep as leading idle-line data. See
# README.md "Open questions" for the full reasoning.
CAPTURE_AFTER_TRIGGER_SECONDS = 5.0
PRE_ROLL_SECONDS = 0.5

# Guards against a command that never produces the triggering UART falling
# edge (bad wiring, dropped byte, wrong command) hanging capture.wait()
# forever and stalling the whole sweep. If the capture hasn't completed
# within this many seconds of sending the command, it's force-stopped and
# logged as a failure for that command; the sweep moves on to the next one.
# Should comfortably exceed PRE_ROLL_SECONDS + CAPTURE_AFTER_TRIGGER_SECONDS
# (the expected duration when triggering works normally).
CAPTURE_WAIT_TIMEOUT_SECONDS = 10.0

# Trims the exported capture to roughly [pre-roll + after-trigger] seconds,
# discarding any extra idle time accumulated between arming the capture and
# the start of the PRE_ROLL_SECONDS sleep. Purely a file-size/tidiness
# convenience; set to None to keep the full untrimmed buffer.
TRIM_DATA_SECONDS = PRE_ROLL_SECONDS + CAPTURE_AFTER_TRIGGER_SECONDS + 0.25

# Delay between sweep commands so effects/colors don't visually bleed into
# each other and the board has time to settle before the next command.
INTER_COMMAND_DELAY_SECONDS = 1.0

# Settle time after sending a pre_command (e.g. setting a color before
# sweeping a Starry Night color-transition command) and before the real,
# captured/triggering command is sent.
PRE_COMMAND_SETTLE_SECONDS = 0.3

# --- Protocol analyzers (decoded output, not raw digital timing) -------------
# capture_sweep.py attaches a UART decoder to the trigger line and a WS2812
# decoder to each LED line, then exports the combined decoded data table
# instead of a raw digital CSV. Analyzer name and setting-label strings below
# are copied verbatim from Saleae's own analyzer source (not the UI) since
# add_analyzer() requires an exact match:
#   https://github.com/saleae/serial-analyzer (Async Serial)
#   https://github.com/saleae/async-rgb-led-analyzer (Addressable LEDs (Async))
# Only channel + protocol-identifying settings are set explicitly; every
# other Async Serial setting (8 bits, 1 stop bit, no parity, LSB first, non
# inverted, Normal mode) already defaults to what this protocol needs.
UART_ANALYZER_NAME = "Async Serial"
WS2812_ANALYZER_NAME = "Addressable LEDs (Async)"

# "LED Controller" dropdown value in the Addressable LEDs (Async) analyzer.
# WS2812B confirmed working via your manual capture; change if a future
# board/revision turns out to need a different entry from that dropdown
# (WS2811, WS2813, TM1809, TM1804, UCS1903, LPD1886 - 24/36 bit).
WS2812_LED_CONTROLLER = "WS2812B"

# --- Sweep content ---------------------------------------------------------------
# Shooting Star commands only produce visible activity on SS-firmware units.
# Default True per the original spec ("I'll be testing against a separate
# SS-firmware unit"). Set False to skip that section entirely (it will be
# logged as skipped, not treated as a failure).
INCLUDE_SHOOTING_STAR_SWEEP = False

# Skip the ambient_effects/ambient_colors/strip_dimmer sweep categories
# entirely if False (e.g. no ambient strip wired up yet on this board).
INCLUDE_AMBIENT_SWEEP = False

# Per-LED-line capture toggles. Set False to skip attaching/exporting that
# line's WS2812 analyzer entirely -- useful if that channel isn't wired up
# yet, or to cut export size when you only care about one row this run. Has
# no effect on which RS232 commands get sent, only on what gets captured;
# when False, that row's *_ROW_CHANNEL value isn't required in config.py.
INCLUDE_TOP_ROW = True
INCLUDE_BOTTOM_ROW = True

# --- Output ------------------------------------------------------------------------
OUTPUT_DIR = "captures"
