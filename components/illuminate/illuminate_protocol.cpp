#include "illuminate_protocol.h"

#include <algorithm>

namespace esphome {
namespace illuminate {
namespace protocol {

namespace {

// Hardware floor/ceiling for both dimmers per the manual: 10-100.
uint8_t clamp_percent(uint8_t percent) { return std::min<uint8_t>(100, std::max<uint8_t>(10, percent)); }

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

std::string shooting_star_command(ShootingStarZone zone, const RGBW &color, ShootingStarDuration duration) {
  return std::string(1, zone_char(zone)) + "," + rgbw_fields(color) + "," +
         std::to_string(static_cast<uint8_t>(duration)) + "*";
}

std::string shooting_star_off_command(ShootingStarZone zone) {
  return shooting_star_command(zone, RGBW{0, 0, 0, 0}, ShootingStarDuration::OFF);
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

}  // namespace protocol
}  // namespace illuminate
}  // namespace esphome
