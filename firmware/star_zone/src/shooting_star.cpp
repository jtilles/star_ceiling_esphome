#include "shooting_star.h"

#include <Arduino.h>

#include "color_utils.h"

namespace star_zone {
namespace shooting_star {

namespace {
// Trail length in pixels behind the streak's leading edge.
constexpr float kTrailPixels = 3.0f;
}  // namespace

void start(ShootingStarSlot &slot, const RGBW &color, uint8_t duration_minutes, uint32_t now_ms, bool single_shot) {
  if (duration_minutes == 0 && !single_shot) {
    slot.firing = false;
    slot.streak_active = false;
    return;
  }
  slot.firing = true;
  slot.single_shot = single_shot;
  slot.color = color;
  slot.stop_at_ms = single_shot ? 0 : now_ms + (uint32_t) duration_minutes * 60000UL;
  slot.streak_active = false;
  slot.next_streak_ms = now_ms;  // fire the first streak immediately
}

bool tick(ShootingStarSlot &slot, RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed) {
  if (!slot.firing) return false;

  if (!slot.single_shot && (int32_t) (now_ms - slot.stop_at_ms) >= 0) {
    slot.firing = false;
    slot.streak_active = false;
    return false;
  }

  if (!slot.streak_active) {
    if ((int32_t) (now_ms - slot.next_streak_ms) < 0) return true;  // firing, waiting for next streak
    slot.streak_active = true;
    slot.streak_start_ms = now_ms;
  }

  // Faster speed -> quicker traversal of the strip.
  uint16_t streak_ms = map(speed, 0, 255, 1800, 350);
  uint32_t elapsed = now_ms - slot.streak_start_ms;
  if (elapsed >= streak_ms) {
    slot.streak_active = false;
    if (slot.single_shot) {
      slot.firing = false;
      return false;
    }
    slot.next_streak_ms = now_ms + random(400, 1600);
    return true;
  }

  float head = (float) elapsed / streak_ms * (count - 1 + kTrailPixels);
  for (uint8_t i = 0; i < count; i++) {
    float dist = head - i;
    if (dist < 0 || dist > kTrailPixels) continue;
    uint8_t bright = (uint8_t) (255.0f * (1.0f - dist / kTrailPixels));
    color_utils::blend_add(pixels[i], color_utils::scale(slot.color, bright));
  }
  return true;
}

}  // namespace shooting_star
}  // namespace star_zone
