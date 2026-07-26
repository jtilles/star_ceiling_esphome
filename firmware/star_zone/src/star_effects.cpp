#include "star_effects.h"

#include <Arduino.h>
#include <math.h>

#include "color_utils.h"
#include "palette.h"

namespace star_zone {
namespace effects {

namespace {

// Resolves "the color" for pixel i: palette override if active, else
// whatever the color-transition layer already computed for that pixel.
RGBW source_color(uint8_t i, uint32_t now_ms, const RGBW *active_color, uint8_t palette_id) {
  if (palette_id != palette::kSolidPaletteId) {
    uint8_t phase = (uint8_t) ((now_ms / 60 + i * 32) & 0xFF);
    return palette::sample(palette_id, phase);
  }
  return active_color[i];
}

// Symmetric triangle wave, 0 at t=0 and t=period, 255 at t=period/2.
uint8_t triangle(uint32_t t, uint16_t period) {
  uint16_t phase = t % period;
  return phase < period / 2 ? (uint8_t) map(phase, 0, period / 2, 0, 255)
                             : (uint8_t) map(phase, period / 2, period, 255, 0);
}

void render_twinkle(RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed, const RGBW *active_color,
                     uint8_t palette_id) {
  uint16_t base_period = map(speed, 0, 255, 2200, 500);
  for (uint8_t i = 0; i < count; i++) {
    uint16_t period = base_period + (color_utils::hash8(i, 0) % 400);
    uint32_t offset = color_utils::hash8(i, 1) * 7UL;
    uint8_t bright = triangle(now_ms + offset, period);
    pixels[i] = color_utils::scale(source_color(i, now_ms, active_color, palette_id), bright);
  }
}

void render_firefly(RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed, const RGBW *active_color,
                     uint8_t palette_id) {
  // Slower, gentler than Twinkle, and only a subset of pixels glow at once.
  uint16_t base_period = map(speed, 0, 255, 5000, 1500);
  for (uint8_t i = 0; i < count; i++) {
    uint16_t period = base_period + (color_utils::hash8(i, 2) % 1500);
    uint32_t offset = color_utils::hash8(i, 3) * 23UL;
    bool active_this_cycle = ((now_ms + offset) / period) % 2 == color_utils::hash8(i, 4) % 2;
    uint8_t bright = active_this_cycle ? triangle(now_ms + offset, period) / 2 + 20 : 0;
    pixels[i] = color_utils::scale(source_color(i, now_ms, active_color, palette_id), bright);
  }
}

void render_breeze(RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed, const RGBW *active_color,
                    uint8_t palette_id) {
  // A brightness wave sweeps across the strip like wind moving through it.
  float period_s = map(speed, 0, 255, 6000, 1200) / 1000.0f;
  float t = now_ms / 1000.0f;
  for (uint8_t i = 0; i < count; i++) {
    float x = (float) i / count;
    float wave = sinf(2 * PI * (x - t / period_s));
    uint8_t bright = (uint8_t) (140 + 115 * wave);
    pixels[i] = color_utils::scale(source_color(i, now_ms, active_color, palette_id), bright);
  }
}

void render_random(RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed, uint8_t /*palette_id*/) {
  uint16_t change_interval = map(speed, 0, 255, 2200, 300);
  for (uint8_t i = 0; i < count; i++) {
    uint32_t bucket = now_ms / change_interval;
    uint8_t hue = color_utils::hash8(i, (uint16_t) bucket);
    pixels[i] = color_utils::hue_to_rgb(hue);
  }
}

void render_rain_drops(RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed, const RGBW *active_color,
                        uint8_t palette_id) {
  // Sharp attack, fast decay per pixel - a "drop" landing then fading.
  uint16_t change_interval = map(speed, 0, 255, 1400, 250);
  constexpr uint16_t kDecayMs = 260;
  for (uint8_t i = 0; i < count; i++) {
    uint32_t bucket = now_ms / change_interval;
    bool drop_here = color_utils::hash8(i, (uint16_t) bucket) > 170;  // ~1/3 chance
    uint32_t since_bucket_start = now_ms % change_interval;
    uint8_t bright = (drop_here && since_bucket_start < kDecayMs)
                          ? (uint8_t) map(since_bucket_start, 0, kDecayMs, 255, 0)
                          : 0;
    pixels[i] = color_utils::scale(source_color(i, now_ms, active_color, palette_id), bright);
  }
}

void render_jamboree(RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed, const RGBW *active_color,
                      uint8_t palette_id) {
  // The energetic one: fast hue rotation across the strip plus twinkle-style
  // sparkle on top.
  uint16_t hue_period = map(speed, 0, 255, 3000, 600);
  uint16_t sparkle_period = map(speed, 0, 255, 900, 250);
  for (uint8_t i = 0; i < count; i++) {
    uint8_t hue = (uint8_t) (((now_ms % hue_period) * 255UL / hue_period) + i * (255 / count));
    RGBW base = palette_id != palette::kSolidPaletteId ? source_color(i, now_ms, active_color, palette_id)
                                                         : color_utils::hue_to_rgb(hue);
    uint8_t sparkle = triangle(now_ms + color_utils::hash8(i, 5) * 11UL, sparkle_period);
    uint8_t bright = (uint8_t) (100 + ((uint16_t) sparkle * 155 / 255));
    pixels[i] = color_utils::scale(base, bright);
  }
}

}  // namespace

void render(StarryEffect effect, RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed,
            const RGBW *active_color, uint8_t palette_id) {
  switch (effect) {
    case StarryEffect::TWINKLE:
      render_twinkle(pixels, count, now_ms, speed, active_color, palette_id);
      return;
    case StarryEffect::FIREFLY:
      render_firefly(pixels, count, now_ms, speed, active_color, palette_id);
      return;
    case StarryEffect::BREEZE:
      render_breeze(pixels, count, now_ms, speed, active_color, palette_id);
      return;
    case StarryEffect::RANDOM:
      render_random(pixels, count, now_ms, speed, palette_id);
      return;
    case StarryEffect::RAIN_DROPS:
      render_rain_drops(pixels, count, now_ms, speed, active_color, palette_id);
      return;
    case StarryEffect::JAMBOREE:
      render_jamboree(pixels, count, now_ms, speed, active_color, palette_id);
      return;
  }
}

}  // namespace effects
}  // namespace star_zone
