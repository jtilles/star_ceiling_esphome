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
  // interval_seconds: 0-65535, 0 = off. Fires one streak immediately, then
  // one more every interval_seconds until turned off - see shooting_star.h.
  void set_shooting_star(ShootingStarZone zone, const RGBW &color, uint16_t interval_seconds);

  // --- Enhancement command handlers (new "!"-prefixed namespace, see
  // protocol.h - guaranteed not to collide with the legacy grammar) ---
  void set_master_brightness(uint8_t percent);
  // Brightness of shooting-star streaks (including meteor shower), 0-100,
  // INDEPENDENT of the Starry Night star dimmer ("D"). Lets a dim ambient
  // starfield carry bright streaks over it. See render_ for where in the
  // pipeline each multiplier lands.
  void set_shooting_star_brightness(uint8_t percent);
  void set_effect_speed(uint8_t speed);
  void set_power(bool on, uint16_t transition_ms);
  void set_identify_mode(bool active);
  void identify_step(int8_t delta);
  void set_palette(uint8_t palette_id);
  void set_meteor_shower(bool active, uint16_t avg_interval_s);
  // trail_pixels: fade length behind the streak's head, clamped to [1,40].
  // speed: 0-255, or 256+ (any out-of-range value) to clear the override
  // and fall back to the shared "!S" effect speed.
  // decay_x100: fade-curve exponent * 100, clamped to [10,500] (0.10-5.00).
  // 100 = pure linear falloff (this firmware's original hardcoded shape,
  // where trail length was the only knob - see shooting_star.cpp); >100
  // makes the tail dim faster near the head (looks shorter for the same
  // trail_pixels); <100 keeps it brighter for longer before dropping off
  // near the very end.
  void set_shooting_star_config(ShootingStarZone zone, uint8_t trail_pixels, uint16_t speed, uint16_t decay_x100);
  // How long the Wipe/Fade color transitions ('w'/'f') take to complete,
  // clamped to [50,10000]ms. Applies to both - Wipe and Fade differ in how
  // they use this duration (see compute_active_color_), not in duration.
  void set_color_transition_speed(uint16_t ms);

 private:
  RGBW compute_active_color_(uint8_t pixel_index, uint32_t now_ms) const;
  void render_(uint32_t now_ms);
  void maybe_fire_meteor_(uint32_t now_ms);
  // Clears the "no command received yet" boot-dark state - called from the
  // top of every public setter (see .cpp) so *any* command wakes the strip.
  void wake_() { booted_dark_ = false; }

  // --- Persistence (EEPROM) - see firmware/star_zone/README.md's
  // "Persistence across power cycles" section for what is and isn't saved
  // and why. Loaded once in begin(); saved (debounced) from tick(). ---
  void load_persisted_state_();
  void save_persisted_state_();
  // Called from every setter below that touches a *persisted* field (not
  // every setter - e.g. identify mode and meteor-shower/shooting-star
  // firing state are deliberately transient, see README). Debounced rather
  // than written immediately so a burst of changes (e.g. someone dragging
  // through several sliders) costs one EEPROM write, not one per command -
  // EEPROM.put() already skips unchanged bytes (Arduino core's update()
  // semantics), but a compare-per-byte pass on every single command still
  // isn't free and there's no reason to pay it that often.
  void mark_dirty_() {
    state_dirty_ = true;
    last_state_change_ms_ = millis();
  }

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
  uint16_t color_transition_ms_{700};  // configurable via "!F", see set_color_transition_speed

  // One independent shooting-star slot per zone (X/Y/Z).
  ShootingStarSlot shooting_star_[kShootingStarZoneCount]{};

  // Enhancements
  uint8_t master_brightness_pct_{100};
  // Applied to shooting-star streaks only, so "D" can dim the starfield
  // without dimming the streaks crossing it. Defaults to 100 (no change
  // from before this existed).
  uint8_t shooting_star_brightness_pct_{100};
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

  bool state_dirty_{false};
  uint32_t last_state_change_ms_{0};
};

// On-disk (EEPROM) layout for persisted settings - see
// StarZoneController::load_persisted_state_/save_persisted_state_ in
// controller.cpp. Deliberately a plain, fixed-layout struct (no versioned
// serialization framework) - kPersistVersion below is the only compat
// mechanism, and that's enough for a single-chip, single-firmware-image
// project like this one.
//
// Field-order note: `version` is first but `magic` is LAST, and that is
// load-bearing, not arbitrary. EEPROM.put() writes byte-by-byte from the
// start of the struct, so if power is lost partway through a save, the
// leading bytes hold new data and the trailing ones hold stale data. With
// the magic byte written last, such a torn record simply never gets its
// magic and is correctly rejected on the next boot. (The original layout
// had magic first, which meant a half-finished save left a record that
// looked perfectly valid but carried stale/garbage fields.)
//
// Power state is deliberately NOT persisted - see the "Persistence across
// power cycles" section in firmware/star_zone/README.md. Restoring "off"
// across a reboot produces a chip that powers up dark for no visible
// reason, which reads as broken hardware rather than as a remembered
// setting.
struct PersistedState {
  uint8_t version;   // must equal kPersistVersion - bumped whenever this
                     // struct's layout changes, so a firmware update with a
                     // different layout ignores stale-format EEPROM instead
                     // of misreading it as valid
  uint8_t effect;
  uint8_t transition;
  uint8_t color_r, color_g, color_b, color_w;
  uint8_t dimmer_pct;
  uint16_t color_transition_ms;
  uint8_t master_brightness_pct;
  uint8_t shooting_star_brightness_pct;
  uint8_t effect_speed;
  uint8_t palette_id;
  uint8_t trail_pixels[kShootingStarZoneCount];
  int16_t speed_override[kShootingStarZoneCount];
  uint16_t decay_x100[kShootingStarZoneCount];
  uint8_t magic;     // must equal kPersistMagic. WRITTEN LAST (see above) -
                     // acts as a commit marker for the whole record, and
                     // distinguishes a real save from blank/erased EEPROM,
                     // which reads as 0xFF per byte.
};
constexpr uint8_t kPersistMagic = 0x5A;
// v2: dropped the persisted `power_on` field and moved `magic` to the end of
//     the struct as a torn-write commit marker.
// v3: added shooting_star_brightness_pct ("!B").
// Older records are a different layout and are correctly ignored by the
// version check rather than misread.
constexpr uint8_t kPersistVersion = 3;
// How long to wait after the last persisted-field change before actually
// writing EEPROM - see mark_dirty_()'s comment above.
constexpr uint32_t kPersistDebounceMs = 3000;

// Generous headroom check against the ATmega328P's 1KB EEPROM - not a tight
// bound, just a guard against an accidental huge addition to PersistedState
// going unnoticed.
static_assert(sizeof(PersistedState) <= 64, "PersistedState has grown unexpectedly large for a 1KB EEPROM part");

}  // namespace star_zone
