#include "shooting_star.h"

#include <Arduino.h>
#include <math.h>

#include "color_utils.h"

namespace star_zone {
namespace shooting_star {

void start(ShootingStarSlot &slot, const RGBW &color, uint16_t interval_seconds, uint32_t now_ms, bool single_shot) {
  if (interval_seconds == 0 && !single_shot) {
    slot.firing = false;
    slot.streak_active = false;
    return;
  }
  slot.firing = true;
  slot.single_shot = single_shot;
  slot.color = color;
  slot.interval_ms = single_shot ? 0 : (uint32_t) interval_seconds * 1000UL;
  slot.streak_active = false;
  slot.next_streak_ms = now_ms;  // fire the first streak immediately
}

bool tick(ShootingStarSlot &slot, RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed) {
  if (!slot.firing) return false;

  if (!slot.streak_active) {
    if ((int32_t) (now_ms - slot.next_streak_ms) < 0) return true;  // firing, waiting for the next scheduled shot
    slot.streak_active = true;
    slot.streak_start_ms = now_ms;
  }

  // Faster speed -> quicker traversal of the strip.
  uint16_t streak_ms = map(speed, 0, 255, 3200, 350);
  uint32_t elapsed = now_ms - slot.streak_start_ms;
  if (elapsed >= streak_ms) {
    slot.streak_active = false;
    if (slot.single_shot) {
      slot.firing = false;
      return false;
    }
    // Fixed cadence from when this streak *started*, not a random gap after
    // it finished - "interval_seconds" is meant to be how often shots fire,
    // not how long a burst of firing lasts. If the streak itself took longer
    // than the requested interval (fast travel is unlikely to, but a very
    // short interval could), fire again immediately rather than drift.
    slot.next_streak_ms = slot.streak_start_ms + slot.interval_ms;
    if ((int32_t) (slot.next_streak_ms - now_ms) < 0) slot.next_streak_ms = now_ms;
    return true;
  }

  float head = (float) elapsed / streak_ms * (count - 1 + slot.trail_pixels);
  for (uint8_t i = 0; i < count; i++) {
    float dist = head - i;
    if (dist < 0 || dist > slot.trail_pixels) continue;
    // linear (0..1, full brightness at the head down to 0 at trail_pixels
    // back) raised to decay_exponent - see the field comment in
    // shooting_star.h. exponent 1.0 reproduces the original pure-linear ramp.
    float linear = 1.0f - dist / slot.trail_pixels;
    uint8_t bright = (uint8_t) (255.0f * powf(linear, slot.decay_exponent));
    color_utils::blend_add(pixels[i], color_utils::scale(slot.color, bright));
  }
  return true;
}

}  // namespace shooting_star
}  // namespace star_zone
