#include "color_utils.h"

namespace star_zone {
namespace color_utils {

namespace {
uint8_t scale_channel(uint8_t v, uint8_t brightness255) { return (uint16_t) v * brightness255 / 255; }
uint8_t lerp_channel(uint8_t a, uint8_t b, uint8_t t255) {
  return (uint8_t) (((uint16_t) a * (255 - t255) + (uint16_t) b * t255) / 255);
}
uint8_t add_channel(uint8_t a, uint8_t b) {
  uint16_t sum = (uint16_t) a + b;
  return sum > 255 ? 255 : (uint8_t) sum;
}
}  // namespace

RGBW scale(const RGBW &c, uint8_t brightness255) {
  return RGBW{scale_channel(c.r, brightness255), scale_channel(c.g, brightness255), scale_channel(c.b, brightness255),
              scale_channel(c.w, brightness255)};
}

RGBW lerp(const RGBW &a, const RGBW &b, uint8_t t255) {
  return RGBW{lerp_channel(a.r, b.r, t255), lerp_channel(a.g, b.g, t255), lerp_channel(a.b, b.b, t255),
              lerp_channel(a.w, b.w, t255)};
}

void blend_add(RGBW &dst, const RGBW &add) {
  dst.r = add_channel(dst.r, add.r);
  dst.g = add_channel(dst.g, add.g);
  dst.b = add_channel(dst.b, add.b);
  dst.w = add_channel(dst.w, add.w);
}

RGBW hue_to_rgb(uint8_t hue255) {
  uint8_t region = hue255 / 43;       // 6 regions of ~43 each (255/6)
  uint8_t remainder = (hue255 % 43) * 6;
  uint8_t q = 255 - remainder;
  uint8_t t = remainder;
  switch (region) {
    case 0:
      return RGBW{255, t, 0, 0};
    case 1:
      return RGBW{q, 255, 0, 0};
    case 2:
      return RGBW{0, 255, t, 0};
    case 3:
      return RGBW{0, q, 255, 0};
    case 4:
      return RGBW{t, 0, 255, 0};
    default:
      return RGBW{255, 0, q, 0};
  }
}

uint8_t hash8(uint16_t a, uint16_t b) {
  // Callers pass a slowly-incrementing "time bucket" as b (RAIN_DROPS/RANDOM
  // in star_effects.cpp), so this needs real avalanche: a +1 change to
  // either input has to scramble every output bit, or consecutive buckets
  // produce nearly the same value and a pixel gets "stuck" on/off for many
  // ticks in a row instead of a fresh random draw each interval. A plain
  // multiply-and-shift doesn't have that property (a naive earlier version
  // of this function packed b's contribution into a 16-bit multiplier,
  // which only nudged the output by a fraction of a unit per +1 - visually,
  // "rain drops" that stay lit on the same few pixels far longer than
  // intended). Thomas Wang's 32-bit integer hash below is a standard,
  // cheap way to get proper avalanche.
  uint32_t h = ((uint32_t) a << 16) | b;
  h = (h ^ 61) ^ (h >> 16);
  h = h + (h << 3);
  h = h ^ (h >> 4);
  h = h * 0x27d4eb2dU;
  h = h ^ (h >> 15);
  return (uint8_t) h;
}

}  // namespace color_utils
}  // namespace star_zone
