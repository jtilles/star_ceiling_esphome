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
constexpr uint8_t kPaletteCount = 4;  // valid ids: 1..kPaletteCount

// Samples palette `id` (1..kPaletteCount) at `phase` (0-255, wraps).
RGBW sample(uint8_t id, uint8_t phase);

}  // namespace palette
}  // namespace star_zone
