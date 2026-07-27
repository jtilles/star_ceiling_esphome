#include "controller.h"

#include <Arduino.h>

#include "color_utils.h"
#include "palette.h"
#include "star_effects.h"

namespace star_zone {

void StarZoneController::begin() {
  strip_.begin();
  strip_.show();  // all off until the first command/frame
  // ADC6 (Arduino A6) is a confirmed always-NC, analog-only pin on this
  // package (docs/illuminate-hardware-notes.md) - a floating pin's noise is
  // a convenient entropy source, mirroring the original firmware's own
  // ADC-based randomSeed()-style read (docs/illuminate-firmware-notes.md,
  // "Animation engine").
  randomSeed(analogRead(A6));
}

void StarZoneController::set_effect(StarryEffect effect) {
  wake_();
  effect_ = effect;
}

void StarZoneController::set_transition(ColorTransition transition) {
  wake_();
  transition_ = transition;
}

void StarZoneController::set_color(const RGBW &color) {
  wake_();
  transition_from_ = color_;
  color_ = color;
  transition_started_ms_ = millis();
}

void StarZoneController::set_dimmer(uint8_t percent) {
  wake_();
  dimmer_pct_ = constrain(percent, kDimmerMinPct, kDimmerMaxPct);
}

void StarZoneController::set_shooting_star(ShootingStarZone zone, const RGBW &color, uint8_t duration_minutes) {
  wake_();
  shooting_star::start(shooting_star_[(uint8_t) zone], color, duration_minutes, millis());
}

void StarZoneController::set_shooting_star_config(ShootingStarZone zone, uint8_t trail_pixels, uint16_t speed) {
  wake_();
  ShootingStarSlot &slot = shooting_star_[(uint8_t) zone];
  slot.trail_pixels = (float) (trail_pixels < 1 ? 1 : (trail_pixels > 40 ? 40 : trail_pixels));
  slot.speed_override = speed > 255 ? -1 : (int16_t) speed;
}

void StarZoneController::set_master_brightness(uint8_t percent) {
  wake_();
  master_brightness_pct_ = percent > 100 ? 100 : percent;
}

void StarZoneController::set_effect_speed(uint8_t speed) {
  wake_();
  effect_speed_ = speed;
}

void StarZoneController::set_power(bool on, uint16_t transition_ms) {
  wake_();
  power_on_ = on;
  power_transition_ms_ = transition_ms == 0 ? 1 : transition_ms;
  power_level_start_ = power_level_;  // interrupts any transition already in flight
  power_level_target_ = on ? 1.0f : 0.0f;
  power_transition_started_ms_ = millis();
}

void StarZoneController::set_identify_mode(bool active) {
  wake_();
  identify_active_ = active;
  if (active) identify_index_ = 0;
}

void StarZoneController::identify_step(int8_t delta) {
  if (!identify_active_) return;
  wake_();
  int16_t idx = (int16_t) identify_index_ + delta;
  idx = ((idx % (int16_t) kLedCount) + (int16_t) kLedCount) % (int16_t) kLedCount;
  identify_index_ = (uint8_t) idx;
}

void StarZoneController::set_palette(uint8_t palette_id) {
  wake_();
  palette_id_ = palette_id > palette::kPaletteCount ? palette::kSolidPaletteId : palette_id;
}

void StarZoneController::set_meteor_shower(bool active, uint16_t avg_interval_s) {
  wake_();
  meteor_shower_active_ = active;
  meteor_shower_interval_s_ = avg_interval_s == 0 ? 1 : avg_interval_s;
  meteor_next_fire_ms_ = millis() + random(500, (uint32_t) meteor_shower_interval_s_ * 1000UL);
}

RGBW StarZoneController::compute_active_color_(uint8_t pixel_index, uint32_t now_ms) const {
  switch (transition_) {
    case ColorTransition::NONE:
      return color_;
    case ColorTransition::FADE: {
      uint32_t elapsed = now_ms - transition_started_ms_;
      if (elapsed >= kColorTransitionMs) return color_;
      uint8_t t = (uint8_t) ((elapsed * 255UL) / kColorTransitionMs);
      return color_utils::lerp(transition_from_, color_, t);
    }
    case ColorTransition::WIPE: {
      uint32_t elapsed = now_ms - transition_started_ms_;
      if (elapsed >= kColorTransitionMs) return color_;
      float progress = (float) elapsed / kColorTransitionMs;
      float boundary = progress * kLedCount;
      return pixel_index < boundary ? color_ : transition_from_;
    }
    case ColorTransition::RAINBOW: {
      uint16_t period_ms = map(effect_speed_, 0, 255, 8000, 800);
      uint8_t hue = (uint8_t) (((now_ms % period_ms) * 255UL / period_ms) + pixel_index * (255 / kLedCount));
      return color_utils::hue_to_rgb(hue);
    }
    case ColorTransition::BLINK: {
      uint16_t period_ms = map(effect_speed_, 0, 255, 1600, 200);
      bool on = (now_ms % period_ms) < (period_ms / 2);
      return on ? color_ : RGBW{0, 0, 0, 0};
    }
  }
  return color_;
}

void StarZoneController::maybe_fire_meteor_(uint32_t now_ms) {
  if (!meteor_shower_active_) return;
  if ((int32_t) (now_ms - meteor_next_fire_ms_) < 0) return;
  RGBW color = palette_id_ != palette::kSolidPaletteId ? palette::sample(palette_id_, (uint8_t) random(0, 256))
                                                         : color_;
  shooting_star::start(meteor_slot_, color, /*duration_minutes=*/0, now_ms, /*single_shot=*/true);
  meteor_next_fire_ms_ = now_ms + random(500, (uint32_t) meteor_shower_interval_s_ * 1000UL + 500);
}

void StarZoneController::tick(uint32_t now_ms) { render_(now_ms); }

void StarZoneController::render_(uint32_t now_ms) {
  if (booted_dark_) {
    for (uint8_t i = 0; i < kLedCount; i++) strip_.setPixelColor(i, 0);
    strip_.show();
    return;
  }

  RGBW active_color[kLedCount];
  for (uint8_t i = 0; i < kLedCount; i++) active_color[i] = compute_active_color_(i, now_ms);

  effects::render(effect_, pixels_, kLedCount, now_ms, effect_speed_, active_color, palette_id_);

  for (auto &slot : shooting_star_) {
    uint8_t zone_speed = slot.speed_override >= 0 ? (uint8_t) slot.speed_override : effect_speed_;
    shooting_star::tick(slot, pixels_, kLedCount, now_ms, zone_speed);
  }

  maybe_fire_meteor_(now_ms);
  shooting_star::tick(meteor_slot_, pixels_, kLedCount, now_ms, effect_speed_);

  if (identify_active_) {
    for (uint8_t i = 0; i < kLedCount; i++) pixels_[i] = (i == identify_index_) ? RGBW{255, 255, 255, 255} : RGBW{};
  }

  // Advance the smooth on/off transition, then compose it with the two
  // manual dimmers into one output multiplier.
  if (now_ms - power_transition_started_ms_ >= power_transition_ms_) {
    power_level_ = power_level_target_;
  } else {
    float t = (float) (now_ms - power_transition_started_ms_) / power_transition_ms_;
    power_level_ = power_level_start_ + (power_level_target_ - power_level_start_) * t;
  }
  float overall = power_level_ * (dimmer_pct_ / 100.0f) * (master_brightness_pct_ / 100.0f);
  uint8_t overall_brightness = (uint8_t) (255.0f * overall);

  for (uint8_t i = 0; i < kLedCount; i++) {
    RGBW out = color_utils::scale(pixels_[i], overall_brightness);
    strip_.setPixelColor(i, strip_.Color(out.r, out.g, out.b, out.w));
  }
  strip_.show();
}

}  // namespace star_zone
