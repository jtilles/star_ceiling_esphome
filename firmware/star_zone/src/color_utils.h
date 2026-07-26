// Small color-math helpers shared by the effects/transitions/shooting-star
// renderers. Kept free of Arduino/NeoPixel types so it's trivially testable.
#pragma once

#include "types.h"

namespace star_zone {
namespace color_utils {

// Scales each channel of `c` by `brightness255` / 255.
RGBW scale(const RGBW &c, uint8_t brightness255);

// Linearly interpolates from `a` (t=0) to `b` (t=255).
RGBW lerp(const RGBW &a, const RGBW &b, uint8_t t255);

// Adds `add` into `dst` in place, clamping each channel at 255. Used to
// layer shooting-star streaks on top of the base Starry Night render
// without one zone's streak simply overwriting another's.
void blend_add(RGBW &dst, const RGBW &add);

// Standard HSV(hue, 1.0, 1.0) -> RGB, hue wrapped to a single byte (0-255
// standing in for 0-359 degrees). Always returns w=0 - callers that want a
// white component mix it in separately.
RGBW hue_to_rgb(uint8_t hue255);

// Cheap deterministic pseudo-random byte from two small integers, without
// touching Arduino's global random() state. Used so per-pixel/per-time-
// bucket "randomness" (e.g. RANDOM/RAIN_DROPS effects) is stable within a
// bucket instead of re-rolling every frame.
uint8_t hash8(uint16_t a, uint16_t b);

}  // namespace color_utils
}  // namespace star_zone
