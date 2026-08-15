#include "palette.h"

#include "color_utils.h"

namespace star_zone {
namespace palette {

namespace {

struct Stop {
  uint8_t phase;
  RGBW color;
};

// 4 stops per palette, phase 0 and 255 should match so the cycle loops
// cleanly. Arbitrary "themed" sets - no hardware reference to match.
const Stop kFire[4] = {
    {0, {255, 40, 0, 0}}, {85, {255, 140, 0, 0}}, {170, {255, 200, 40, 0}}, {255, {255, 40, 0, 0}}};
const Stop kOcean[4] = {
    {0, {0, 40, 120, 0}}, {85, {0, 140, 220, 0}}, {170, {80, 220, 255, 0}}, {255, {0, 40, 120, 0}}};
const Stop kForest[4] = {
    {0, {10, 60, 10, 0}}, {85, {40, 160, 40, 0}}, {170, {120, 200, 60, 0}}, {255, {10, 60, 10, 0}}};
const Stop kCandy[4] = {
    {0, {255, 20, 120, 0}}, {85, {200, 60, 255, 0}}, {170, {255, 180, 220, 0}}, {255, {255, 20, 120, 0}}};

const Stop *palette_stops(uint8_t id) {
  switch (id) {
    case 1:
      return kFire;
    case 2:
      return kOcean;
    case 3:
      return kForest;
    default:
      return kCandy;
  }
}

}  // namespace

RGBW sample(uint8_t id, uint8_t phase) {
  // Rainbow Cycle isn't a 4-stop gradient - it's the full spectrum, so map
  // phase straight onto hue. See kRainbowCycleId's comment in palette.h.
  if (id == kRainbowCycleId) return color_utils::hue_to_rgb(phase);

  const Stop *stops = palette_stops(id);
  uint8_t seg = 0;
  while (seg < 2 && phase >= stops[seg + 1].phase) seg++;
  uint8_t span = stops[seg + 1].phase - stops[seg].phase;
  uint8_t t = span == 0 ? 0 : (uint8_t) (((uint16_t) (phase - stops[seg].phase) * 255) / span);
  return color_utils::lerp(stops[seg].color, stops[seg + 1].color, t);
}

}  // namespace palette
}  // namespace star_zone
