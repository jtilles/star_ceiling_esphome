#include "controller.h"

#include <Arduino.h>
#include <EEPROM.h>

#include "color_utils.h"
#include "palette.h"
#include "star_effects.h"

namespace star_zone {

void StarZoneController::begin() {
  strip_.begin();
  strip_.show();  // all off until the first command/frame, or until
                   // load_persisted_state_() finds a valid saved state
  // ADC6 (Arduino A6) is a confirmed always-NC, analog-only pin on this
  // package (docs/illuminate-hardware-notes.md) - a floating pin's noise is
  // a convenient entropy source, mirroring the original firmware's own
  // ADC-based randomSeed()-style read (docs/illuminate-firmware-notes.md,
  // "Animation engine").
  randomSeed(analogRead(A6));
  load_persisted_state_();
}

void StarZoneController::set_effect(StarryEffect effect) {
  wake_();
  effect_ = effect;
  mark_dirty_();
}

void StarZoneController::set_transition(ColorTransition transition) {
  wake_();
  transition_ = transition;
  mark_dirty_();
}

void StarZoneController::set_color(const RGBW &color) {
  wake_();
  transition_from_ = color_;
  color_ = color;
  transition_started_ms_ = millis();
  mark_dirty_();
}

void StarZoneController::set_dimmer(uint8_t percent) {
  wake_();
  dimmer_pct_ = constrain(percent, kDimmerMinPct, kDimmerMaxPct);
  mark_dirty_();
}

void StarZoneController::set_shooting_star(ShootingStarZone zone, const RGBW &color, uint16_t interval_seconds) {
  wake_();
  shooting_star::start(shooting_star_[(uint8_t) zone], color, interval_seconds, millis());
}

void StarZoneController::set_shooting_star_config(ShootingStarZone zone, uint8_t trail_pixels, uint16_t speed,
                                                    uint16_t decay_x100) {
  wake_();
  ShootingStarSlot &slot = shooting_star_[(uint8_t) zone];
  slot.trail_pixels = (float) (trail_pixels < 1 ? 1 : (trail_pixels > 40 ? 40 : trail_pixels));
  slot.speed_override = speed > 255 ? -1 : (int16_t) speed;
  uint16_t decay_clamped = decay_x100 < 10 ? 10 : (decay_x100 > 500 ? 500 : decay_x100);
  slot.decay_exponent = decay_clamped / 100.0f;
  mark_dirty_();
}

void StarZoneController::set_color_transition_speed(uint16_t ms) {
  wake_();
  color_transition_ms_ = ms < 50 ? 50 : (ms > 10000 ? 10000 : ms);
  mark_dirty_();
}

void StarZoneController::set_master_brightness(uint8_t percent) {
  wake_();
  master_brightness_pct_ = percent > 100 ? 100 : percent;
  mark_dirty_();
}

void StarZoneController::set_effect_speed(uint8_t speed) {
  wake_();
  effect_speed_ = speed;
  mark_dirty_();
}

void StarZoneController::set_power(bool on, uint16_t transition_ms) {
  wake_();
  power_on_ = on;
  power_transition_ms_ = transition_ms == 0 ? 1 : transition_ms;
  power_level_start_ = power_level_;  // interrupts any transition already in flight
  power_level_target_ = on ? 1.0f : 0.0f;
  power_transition_started_ms_ = millis();
  mark_dirty_();
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
  mark_dirty_();
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
      if (elapsed >= color_transition_ms_) return color_;
      uint8_t t = (uint8_t) ((elapsed * 255UL) / color_transition_ms_);
      return color_utils::lerp(transition_from_, color_, t);
    }
    case ColorTransition::WIPE: {
      uint32_t elapsed = now_ms - transition_started_ms_;
      if (elapsed >= color_transition_ms_) return color_;
      float progress = (float) elapsed / color_transition_ms_;
      float boundary = progress * kLedCount;
      return pixel_index < boundary ? color_ : transition_from_;
    }
    case ColorTransition::RAINBOW: {
      uint16_t period_ms = map(effect_speed_, 0, 255, 14000, 800);
      uint8_t hue = (uint8_t) (((now_ms % period_ms) * 255UL / period_ms) + pixel_index * (255 / kLedCount));
      return color_utils::hue_to_rgb(hue);
    }
    case ColorTransition::BLINK: {
      uint16_t period_ms = map(effect_speed_, 0, 255, 3000, 200);
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
  shooting_star::start(meteor_slot_, color, /*interval_seconds=*/0, now_ms, /*single_shot=*/true);
  meteor_next_fire_ms_ = now_ms + random(500, (uint32_t) meteor_shower_interval_s_ * 1000UL + 500);
}

void StarZoneController::tick(uint32_t now_ms) {
  render_(now_ms);
  if (state_dirty_ && (now_ms - last_state_change_ms_) >= kPersistDebounceMs) {
    save_persisted_state_();
    state_dirty_ = false;
  }
}

void StarZoneController::load_persisted_state_() {
  PersistedState saved;
  EEPROM.get(0, saved);
  // Blank/erased EEPROM (reads as 0xFF per byte), a chip that's never saved
  // anything, or a stale layout from an older firmware version - in any of
  // those cases there's nothing valid to restore, so leave every field at
  // its normal power-on default and booted_dark_ stays true as before.
  if (saved.magic != kPersistMagic || saved.version != kPersistVersion) return;

  effect_ = (StarryEffect) saved.effect;
  transition_ = (ColorTransition) saved.transition;
  color_ = RGBW{saved.color_r, saved.color_g, saved.color_b, saved.color_w};
  transition_from_ = color_;  // no in-flight transition to resume across a power cycle
  dimmer_pct_ = saved.dimmer_pct;
  color_transition_ms_ = saved.color_transition_ms;
  master_brightness_pct_ = saved.master_brightness_pct;
  effect_speed_ = saved.effect_speed;
  power_on_ = saved.power_on != 0;
  // Resume instantly at the restored on/off state - no fade-in/out, that's
  // only meaningful for a live "!O" toggle, not a power-cycle restore.
  power_level_ = power_level_start_ = power_level_target_ = power_on_ ? 1.0f : 0.0f;
  palette_id_ = saved.palette_id;
  for (uint8_t i = 0; i < kShootingStarZoneCount; i++) {
    shooting_star_[i].trail_pixels = (float) saved.trail_pixels[i];
    shooting_star_[i].speed_override = saved.speed_override[i];
    shooting_star_[i].decay_exponent = saved.decay_x100[i] / 100.0f;
  }
  // Valid saved state - render it immediately instead of waiting for the
  // first serial command, which is the whole point of this feature.
  booted_dark_ = false;
}

void StarZoneController::save_persisted_state_() {
  PersistedState state;
  state.magic = kPersistMagic;
  state.version = kPersistVersion;
  state.effect = (uint8_t) effect_;
  state.transition = (uint8_t) transition_;
  state.color_r = color_.r;
  state.color_g = color_.g;
  state.color_b = color_.b;
  state.color_w = color_.w;
  state.dimmer_pct = dimmer_pct_;
  state.color_transition_ms = color_transition_ms_;
  state.master_brightness_pct = master_brightness_pct_;
  state.effect_speed = effect_speed_;
  state.power_on = power_on_ ? 1 : 0;
  state.palette_id = palette_id_;
  for (uint8_t i = 0; i < kShootingStarZoneCount; i++) {
    state.trail_pixels[i] = (uint8_t) shooting_star_[i].trail_pixels;
    state.speed_override[i] = shooting_star_[i].speed_override;
    state.decay_x100[i] = (uint16_t) (shooting_star_[i].decay_exponent * 100.0f);
  }
  // EEPROM.put() writes byte-by-byte via EEPROM.update() under the hood
  // (Arduino AVR core), which skips any byte that already holds the value
  // being written - so this is wear-safe even though it's called on every
  // debounced save rather than diffing ourselves first.
  EEPROM.put(0, state);
}

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
