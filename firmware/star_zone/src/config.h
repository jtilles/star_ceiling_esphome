// Hardware constants for the Top-Star / Bot-Star replacement firmware.
//
// Pin/LED-count values are transcribed from docs/illuminate-hardware-notes.md
// and docs/illuminate-enhanced-pinmap.md (confirmed via continuity trace).
// Values marked "VERIFY" are best-effort defaults that could not be confirmed
// from the reverse-engineering docs alone and must be checked against real
// hardware during bring-up - see the README's "Open questions before you
// flash a real chip" section.
#pragma once

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

namespace star_zone {

// PD6 / physical pin 10 - confirmed single-wire bit-banged LED data output on
// both Top-Star and Bot-Star (docs/illuminate-hardware-notes.md).
constexpr uint8_t kLedDataPin = 6;

// Each zone (top row / bottom row) drives 8 LEDs
// (docs/illuminate-enhanced-pinmap.md: "Top row and Bottom row (8 LEDs
// each)").
constexpr uint16_t kLedCount = 8;

// VERIFY: the original protocol carries 4-channel RGBW colors (see
// components/illuminate/illuminate_protocol.h), which is why this firmware
// assumes 4-channel SK6812-style RGBW pixels rather than 3-channel WS2812B.
// The hardware notes only confirm "WS2812-style" single-wire timing, not
// channel count. If bring-up shows only 3 channels are actually wired,
// switch this to a 3-channel NeoPixel type and fold W into the RGB mix
// instead.
//
// VERIFY: byte order on the wire. docs/illuminate-enhanced-prd.md lists this
// as an explicit open question (#3). A quick read of the existing
// captures/starry_colors/*.json + *.csv pairs (sent command vs. decoded
// pixel, decoded with a 3-channel WS2812B-order analyzer) is inconsistent
// with a clean GRB or RGB mapping once you account for the channel-count
// mismatch above - that decode isn't trustworthy evidence for a 4-channel
// part. Treat NEO_GRBW below as a starting guess (it's the common SK6812
// default), send a known solid color (e.g. `c,255,0,0,0*`), and correct this
// constant if the observed color doesn't match before trusting anything else
// this firmware renders.
constexpr neoPixelType kLedColorOrder = NEO_GRBW + NEO_KHZ800;

// PD0 / physical pin 30, hardware USART RX. Confirmed as the command input
// for both Top-Star and Bot-Star by the Ghidra decompile
// (docs/illuminate-firmware-notes.md: "The ESP/Bluetooth module's TX line
// lands on PD0 (RXD) on all chips"). Matches the ESPHome component's baud
// rate (components/illuminate/README wiring section / examples/example.yaml).
constexpr uint32_t kSerialBaud = 9600;

// Hardware floor/ceiling for both dimmers per the manual and
// components/illuminate/illuminate_protocol.cpp's clamp_percent().
constexpr uint8_t kDimmerMinPct = 10;
constexpr uint8_t kDimmerMaxPct = 100;

// Longest legacy or enhancement command line this firmware needs to buffer,
// e.g. "!W,1,120*" or "X,255,255,255,255,10*". Generous headroom included.
constexpr uint8_t kCommandBufferSize = 32;

}  // namespace star_zone
