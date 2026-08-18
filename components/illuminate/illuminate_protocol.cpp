#include "illuminate_protocol.h"

#include <algorithm>

namespace esphome {
namespace illuminate {
namespace protocol {

namespace {

// Hardware floor/ceiling for both dimmers per the manual: 10-100.
uint8_t clamp_percent(uint8_t percent) { return std::min<uint8_t>(100, std::max<uint8_t>(10, percent)); }

// The enhancement brightnesses ("!M"/"!B") allow a true 0, unlike the dimmers above.
uint8_t clamp_percent_to_zero(uint8_t percent) { return std::min<uint8_t>(100, percent); }

// Hardware floor/ceiling for ambient effect speed per the manual: 150 (slow) - 255 (fast).
uint8_t clamp_speed(uint8_t speed) { return std::min<uint8_t>(255, std::max<uint8_t>(150, speed)); }

char zone_char(ShootingStarZone zone) {
  switch (zone) {
    case ShootingStarZone::ZONE_1:
      return 'X';
    case ShootingStarZone::ZONE_2:
      return 'Y';
    case ShootingStarZone::ZONE_3:
      return 'Z';
  }
  return 'X';
}

std::string rgbw_fields(const RGBW &color) {
  return std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + "," +
         std::to_string(color.w);
}

}  // namespace

std::string starry_effect_command(StarryEffect effect) {
  switch (effect) {
    case StarryEffect::TWINKLE:
      return "T";
    case StarryEffect::FIREFLY:
      return "F";
    case StarryEffect::BREEZE:
      return "B";
    case StarryEffect::RANDOM:
      return "R";
    case StarryEffect::RAIN_DROPS:
      return "P";
    case StarryEffect::JAMBOREE:
      return "J";
  }
  return "";
}

std::string color_transition_command(ColorTransition transition) {
  switch (transition) {
    case ColorTransition::WIPE:
      return "w";
    case ColorTransition::FADE:
      return "f";
    case ColorTransition::RAINBOW:
      return "r";
    case ColorTransition::BLINK:
      return "b";
  }
  return "";
}

std::string starry_color_command(const RGBW &color) { return "c," + rgbw_fields(color) + "*"; }

std::string star_dimmer_command(uint8_t percent) { return "D," + std::to_string(clamp_percent(percent)) + "*"; }

std::string shooting_star_command(ShootingStarZone zone, const RGBW &color, uint16_t interval_seconds) {
  return std::string(1, zone_char(zone)) + "," + rgbw_fields(color) + "," + std::to_string(interval_seconds) + "*";
}

std::string shooting_star_off_command(ShootingStarZone zone) {
  return shooting_star_command(zone, RGBW{0, 0, 0, 0}, 0);
}

std::string ambient_effect_command(AmbientEffect effect, uint8_t speed) {
  switch (effect) {
    case AmbientEffect::FADE:
      return "e," + std::to_string(clamp_speed(speed));
    case AmbientEffect::BLINK:
      return "k," + std::to_string(clamp_speed(speed));
    case AmbientEffect::NO_EFFECT:
      return "n";
  }
  return "n";
}

std::string ambient_color_command(const RGBW &color) { return "A," + rgbw_fields(color) + "*"; }

std::string ambient_dimmer_command(uint8_t percent) { return "d," + std::to_string(clamp_percent(percent)) + "*"; }

// --- Enhancement commands (star_zone replacement firmware only) -------------

std::string master_brightness_command(uint8_t percent) {
  return "!M," + std::to_string(clamp_percent_to_zero(percent)) + "*";
}

std::string shooting_star_brightness_command(uint8_t percent) {
  return "!B," + std::to_string(clamp_percent_to_zero(percent)) + "*";
}

std::string effect_speed_command(uint8_t speed) { return "!S," + std::to_string(speed) + "*"; }

std::string power_command(bool on, uint16_t transition_ms) {
  return std::string("!O,") + (on ? "1" : "0") + "," + std::to_string(transition_ms) + "*";
}

std::string identify_command(bool enabled) { return std::string("!I,") + (enabled ? "1" : "0") + "*"; }

std::string identify_next_command() { return "!N*"; }

std::string identify_previous_command() { return "!V*"; }

std::string palette_command(StarPalette palette) {
  uint8_t id = std::min<uint8_t>(static_cast<uint8_t>(StarPalette::RAINBOW_CYCLE), static_cast<uint8_t>(palette));
  return "!C," + std::to_string(id) + "*";
}

std::string meteor_shower_command(bool on, uint16_t avg_interval_seconds) {
  return std::string("!W,") + (on ? "1" : "0") + "," + std::to_string(std::max<uint16_t>(1, avg_interval_seconds)) +
         "*";
}

std::string shooting_star_tuning_command(ShootingStarZone zone, uint8_t trail, uint16_t speed, uint16_t decay) {
  uint8_t clamped_trail = std::min<uint8_t>(40, std::max<uint8_t>(1, trail));
  // Anything at or above the sentinel means "no override"; pass it through untouched so
  // the firmware clears the zone's override rather than pinning it to 255 (max speed).
  uint16_t wire_speed = speed >= SHOOTING_STAR_SPEED_INHERIT ? SHOOTING_STAR_SPEED_INHERIT : speed;
  uint16_t clamped_decay = std::min<uint16_t>(500, std::max<uint16_t>(10, decay));
  return std::string("!L,") + zone_char(zone) + "," + std::to_string(clamped_trail) + "," +
         std::to_string(wire_speed) + "," + std::to_string(clamped_decay) + "*";
}

std::string color_transition_duration_command(uint16_t milliseconds) {
  return "!F," + std::to_string(std::min<uint16_t>(10000, std::max<uint16_t>(50, milliseconds))) + "*";
}

}  // namespace protocol
}  // namespace illuminate
}  // namespace esphome
