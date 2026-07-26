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
  uint32_t h = (uint32_t) a * 2654435761u + (uint32_t) b * 40503u;
  return (uint8_t) (h >> 16);
}

}  // namespace color_utils
}  // namespace star_zone
