// StarZoneController: all runtime state for one Top-Star or Bot-Star chip
// (both run this exact same firmware image - see docs/illuminate-hardware-
// notes.md, "TOP-STAR and BOT-STAR run the exact same firmware image").
//
// protocol.cpp parses incoming serial bytes and calls straight into this
// class's public methods; main.cpp calls tick() once per loop() iteration
// to render and push the current frame.
#pragma once

#include <Adafruit_NeoPixel.h>

#include "config.h"
#include "shooting_star.h"
#include "types.h"

namespace star_zone {

class StarZoneController {
 public:
  explicit StarZoneController(Adafruit_NeoPixel &strip) : strip_(strip) {}

  void begin();
  void tick(uint32_t now_ms);

  // --- Legacy command handlers (byte-compatible with the original board /
  // components/illuminate/illuminate_protocol.h) ---
  void set_effect(StarryEffect effect);
  void set_transition(ColorTransition transition);
  void set_color(const RGBW &color);
  void set_dimmer(uint8_t percent);
  void set_shooting_star(ShootingStarZone zone, const RGBW &color, uint8_t duration_minutes);

  // --- Enhancement command handlers (new "!"-prefixed namespace, see
  // protocol.h - guaranteed not to collide with the legacy grammar) ---
  void set_master_brightness(uint8_t percent);
  void set_effect_speed(uint8_t speed);
  void set_power(bool on, uint16_t transition_ms);
  void set_identify_mode(bool active);
  void identify_step(int8_t delta);
  void set_palette(uint8_t palette_id);
  void set_meteor_shower(bool active, uint16_t avg_interval_s);

 private:
  RGBW compute_active_color_(uint8_t pixel_index, uint32_t now_ms) const;
  void render_(uint32_t now_ms);
  void maybe_fire_meteor_(uint32_t now_ms);
  // Clears the "no command received yet" boot-dark state - called from the
  // top of every public setter (see .cpp) so *any* command wakes the strip.
  void wake_() { booted_dark_ = false; }

  Adafruit_NeoPixel &strip_;
  RGBW pixels_[kLedCount]{};

  // Stays true from power-on until the first command of any kind is
  // received, holding the strip fully off. Without this, the strip powers
  // up straight into rendering the default Twinkle effect on the default
  // color, which reads as an unwanted, unprompted "blinking heartbeat"
  // before anything has actually been commanded.
  bool booted_dark_{true};

  // Legacy state
  StarryEffect effect_{StarryEffect::TWINKLE};
  ColorTransition transition_{ColorTransition::NONE};
  RGBW color_{255, 200, 150, 0};
  uint8_t dimmer_pct_{100};

  // Color-transition animation state (drives compute_active_color_).
  RGBW transition_from_{0, 0, 0, 0};
  uint32_t transition_started_ms_{0};
  static constexpr uint16_t kColorTransitionMs = 700;

  // One independent shooting-star slot per zone (X/Y/Z).
  ShootingStarSlot shooting_star_[kShootingStarZoneCount]{};

  // Enhancements
  uint8_t master_brightness_pct_{100};
  uint8_t effect_speed_{128};

  bool power_on_{true};
  uint16_t power_transition_ms_{500};
  float power_level_{1.0f};         // currently-applied 0..1 output multiplier
  float power_level_start_{1.0f};   // value at the start of the current transition
  float power_level_target_{1.0f};  // value the transition is moving toward
  uint32_t power_transition_started_ms_{0};

  bool identify_active_{false};
  uint8_t identify_index_{0};

  uint8_t palette_id_{0};  // palette::kSolidPaletteId = disabled

  bool meteor_shower_active_{false};
  uint16_t meteor_shower_interval_s_{20};
  uint32_t meteor_next_fire_ms_{0};
  ShootingStarSlot meteor_slot_;
};

}  // namespace star_zone
