// Built-in color palettes for the "!C" palette-driven-twinkle enhancement
// (docs/illuminate-enhanced-prd.md: "Palette-driven twinkle: twinkle/sparkle
// effects can use a themed color palette instead of a single fixed color").
// This is new, additive behavior with no original-hardware reference to
// match - the palettes themselves are an arbitrary starting set.
#pragma once

#include "types.h"

namespace star_zone {
namespace palette {

// Palette id 0 means "no palette - use the actively commanded solid color"
// and is handled by the caller; it is never passed into sample().
constexpr uint8_t kSolidPaletteId = 0;
constexpr uint8_t kPaletteCount = 5;  // valid ids: 1..kPaletteCount

// Rainbow Cycle is a special case, not a 4-stop gradient like the others:
// star_effects.cpp's source_color() gives it a phase that is uniform across
// the whole strip and advances with the "!S" effect speed, so every LED
// shows the SAME hue at any instant and the strip fades smoothly through
// the spectrum together. (Contrast the "r" RAINBOW color transition, which
// deliberately offsets hue per pixel to produce a rainbow spread *across*
// the strip - a different look entirely.) sample() still returns a plain
// hue sweep for it, so palette-sampling callers that pick a random phase -
// e.g. the meteor shower - just get a random spectrum color.
constexpr uint8_t kRainbowCycleId = 5;

// Samples palette `id` (1..kPaletteCount) at `phase` (0-255, wraps).
RGBW sample(uint8_t id, uint8_t phase);

}  // namespace palette
}  // namespace star_zone
