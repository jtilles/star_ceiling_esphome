#include "controller.h"

#include <Arduino.h>
#include <EEPROM.h>
#include <stddef.h>  // offsetof, for the two-phase EEPROM commit in save_persisted_state_

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

void StarZoneController::set_shooting_star_brightness(uint8_t percent) {
  wake_();
  shooting_star_brightness_pct_ = percent > 100 ? 100 : percent;
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
  // Deliberately no mark_dirty_() - power is transient, not a persisted
  // setting. A chip that boots dark because it remembered "off" is
  // indistinguishable from a chip that has failed. See PersistedState.
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
  // anything, a stale layout from an older firmware version, or a save torn
  // in half by power loss (magic is written last, so a partial record never
  // gets one) - in all of those cases there's nothing trustworthy to
  // restore, so leave every field at its power-on default and let
  // booted_dark_ stay true as before.
  if (saved.magic != kPersistMagic || saved.version != kPersistVersion) return;

  // EVERY field below goes through the same validation the corresponding
  // setter applies. EEPROM contents are untrusted input: a record can be
  // valid-looking but stale, or corrupted by a marginal cell, and assigning
  // it raw can produce states no command sequence can reach - e.g.
  // dimmer_pct_ = 0 (permanently dark, and NOT recoverable with "!O,1*"
  // since power level and dimmer are separate multipliers), or an
  // out-of-range effect_ enum that matches no case in effects::render() and
  // therefore leaves the pixel buffer untouched forever. Silently falling
  // back to a default beats bricking the strip until someone reflashes.
  if (saved.effect <= (uint8_t) StarryEffect::JAMBOREE) effect_ = (StarryEffect) saved.effect;
  if (saved.transition <= (uint8_t) ColorTransition::BLINK) transition_ = (ColorTransition) saved.transition;
  color_ = RGBW{saved.color_r, saved.color_g, saved.color_b, saved.color_w};  // any 0-255 byte is valid
  transition_from_ = color_;  // no in-flight transition to resume across a power cycle
  dimmer_pct_ = constrain(saved.dimmer_pct, kDimmerMinPct, kDimmerMaxPct);
  color_transition_ms_ = saved.color_transition_ms < 50 ? 50
                          : (saved.color_transition_ms > 10000 ? 10000 : saved.color_transition_ms);
  master_brightness_pct_ = saved.master_brightness_pct > 100 ? 100 : saved.master_brightness_pct;
  shooting_star_brightness_pct_ =
      saved.shooting_star_brightness_pct > 100 ? 100 : saved.shooting_star_brightness_pct;
  effect_speed_ = saved.effect_speed;  // full 0-255 range is valid by construction
  palette_id_ = saved.palette_id > palette::kPaletteCount ? palette::kSolidPaletteId : saved.palette_id;
  for (uint8_t i = 0; i < kShootingStarZoneCount; i++) {
    uint8_t trail = saved.trail_pixels[i];
    shooting_star_[i].trail_pixels = (float) (trail < 1 ? 1 : (trail > 40 ? 40 : trail));
    shooting_star_[i].speed_override =
        (saved.speed_override[i] >= 0 && saved.speed_override[i] <= 255) ? saved.speed_override[i] : -1;
    uint16_t decay = saved.decay_x100[i];
    shooting_star_[i].decay_exponent = (decay < 10 ? 10 : (decay > 500 ? 500 : decay)) / 100.0f;
  }

  // Power state is intentionally not restored - the chip always comes up ON
  // (see PersistedState's comment and the README). Restoring "off" produced
  // a strip that boots dark with no way to tell whether it was remembering a
  // setting or had failed.
  power_on_ = true;
  power_level_ = power_level_start_ = power_level_target_ = 1.0f;

  // Valid saved state - render it immediately instead of waiting for the
  // first serial command, which is the whole point of this feature.
  booted_dark_ = false;
}

void StarZoneController::save_persisted_state_() {
  PersistedState state;
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
  state.shooting_star_brightness_pct = shooting_star_brightness_pct_;
  state.effect_speed = effect_speed_;
  state.palette_id = palette_id_;
  for (uint8_t i = 0; i < kShootingStarZoneCount; i++) {
    state.trail_pixels[i] = (uint8_t) shooting_star_[i].trail_pixels;
    state.speed_override[i] = shooting_star_[i].speed_override;
    state.decay_x100[i] = (uint16_t) (shooting_star_[i].decay_exponent * 100.0f);
  }

  // Two-phase write so a power loss mid-save can't leave a record that looks
  // valid but isn't. Phase 1: invalidate the existing magic, so the record is
  // explicitly "not committed" for the whole time the payload is in flux.
  // Phase 2: write the payload. Phase 3: stamp the magic, committing it. Any
  // interruption leaves the magic absent and the record correctly ignored on
  // the next boot.
  //
  // EEPROM.put() writes byte-by-byte via update() semantics (Arduino AVR
  // core), skipping bytes that already hold the target value, so this stays
  // wear-friendly despite the extra magic write - in the common case where
  // nothing actually changed, phases 1 and 3 are the only cells touched, and
  // phase 2 writes nothing at all.
  const uint8_t magic_offset = (uint8_t) offsetof(PersistedState, magic);
  const uint8_t invalid_magic = 0x00;
  EEPROM.put(magic_offset, invalid_magic);
  state.magic = invalid_magic;  // keep the in-RAM copy consistent with phase 1
  EEPROM.put(0, state);
  EEPROM.put(magic_offset, kPersistMagic);
}

void StarZoneController::render_(uint32_t now_ms) {
  if (booted_dark_) {
    for (uint8_t i = 0; i < kLedCount; i++) strip_.setPixelColor(i, 0);
    strip_.show();
    return;
  }

  RGBW active_color[kLedCount];
  for (uint8_t i = 0; i < kLedCount; i++) active_color[i] = compute_active_color_(i, now_ms);

  // Brightness pipeline, in order. Which multiplier lands where is the whole
  // point - "D" (star dimmer) is applied to the Starry Night layer ALONE,
  // before streaks are blended on, so a dim ambient starfield can carry
  // bright shooting stars across it. Only the master brightness ("!M") and
  // the on/off power level apply to the composited result.
  //
  //   1. effects::render()                -> Starry Night layer, full scale
  //   2. x dimmer_pct_ ("D")              -> starfield only
  //   3. + shooting stars, scaled by      -> streaks, independent of "D"
  //        shooting_star_brightness_pct_ ("!B")
  //   4. identify mode overrides everything if active
  //   5. x master_brightness_pct_ ("!M") x power_level_ ("!O") -> output
  //
  // NOTE: this deliberately changes what "D" affects. It used to be applied
  // at the very end, dimming streaks along with the starfield, which made a
  // dim-background/bright-streak look impossible. Stock hardware has no
  // Shooting Star support at all, so there's no compatibility concern.
  effects::render(effect_, pixels_, kLedCount, now_ms, effect_speed_, active_color, palette_id_);

  if (dimmer_pct_ < 100) {
    uint8_t star_scale = (uint8_t) (255.0f * (dimmer_pct_ / 100.0f));
    for (uint8_t i = 0; i < kLedCount; i++) pixels_[i] = color_utils::scale(pixels_[i], star_scale);
  }

  for (auto &slot : shooting_star_) {
    uint8_t zone_speed = slot.speed_override >= 0 ? (uint8_t) slot.speed_override : effect_speed_;
    shooting_star::tick(slot, pixels_, kLedCount, now_ms, zone_speed, shooting_star_brightness_pct_);
  }

  maybe_fire_meteor_(now_ms);
  shooting_star::tick(meteor_slot_, pixels_, kLedCount, now_ms, effect_speed_, shooting_star_brightness_pct_);

  if (identify_active_) {
    for (uint8_t i = 0; i < kLedCount; i++) pixels_[i] = (i == identify_index_) ? RGBW{255, 255, 255, 255} : RGBW{};
  }

  // Advance the smooth on/off transition, then compose it with the master
  // brightness into one final output multiplier. (dimmer_pct_ is NOT here -
  // it was already applied to the starfield layer above.)
  if (now_ms - power_transition_started_ms_ >= power_transition_ms_) {
    power_level_ = power_level_target_;
  } else {
    float t = (float) (now_ms - power_transition_started_ms_) / power_transition_ms_;
    power_level_ = power_level_start_ + (power_level_target_ - power_level_start_) * t;
  }
  float overall = power_level_ * (master_brightness_pct_ / 100.0f);
  uint8_t overall_brightness = (uint8_t) (255.0f * overall);

  for (uint8_t i = 0; i < kLedCount; i++) {
    RGBW out = color_utils::scale(pixels_[i], overall_brightness);
    strip_.setPixelColor(i, strip_.Color(out.r, out.g, out.b, out.w));
  }
  strip_.show();
}

}  // namespace star_zone
