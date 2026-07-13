// Pure command-string layer for the Fusion Synergy "Illuminate 1.0" RS232 protocol.
//
// This header has NO ESPHome dependencies (no esphome.h, no Component/UARTDevice, no
// Arduino types) so it can be compiled and unit-tested completely independently of the
// ESPHome build (see tests/test_illuminate_protocol.cpp). All ESPHome plumbing lives in
// illuminate.h / illuminate.cpp and the light/shooting_star platform files, which call
// into these free functions.
//
// Command shapes are transcribed directly from the official Illuminate 1.0 manual and
// validated against real hardware for Starry Night effects/colors (see README for what
// has and hasn't been bench-tested).
#pragma once

#include <cstdint>
#include <string>

namespace esphome {
namespace illuminate {

// Starry Night animation effects (single ASCII character, no terminator).
enum class StarryEffect : uint8_t {
  TWINKLE,
  FIREFLY,
  BREEZE,
  RANDOM,
  RAIN_DROPS,
  JAMBOREE,
};

// Starry Night color transitions (single ASCII character, no terminator).
// These are a modifier on top of the active color, not a standalone effect.
enum class ColorTransition : uint8_t {
  WIPE,
  FADE,
  RAINBOW,
  BLINK,
};

// Ambient Light effects. NO_EFFECT ("n") takes no speed argument.
enum class AmbientEffect : uint8_t {
  FADE,
  BLINK,
  NO_EFFECT,
};

// Shooting Star zones, only functional on SS-firmware units.
enum class ShootingStarZone : uint8_t {
  ZONE_1,  // "X" / SS1
  ZONE_2,  // "Y" / SS2
  ZONE_3,  // "Z" / SS3
};

// Shooting Star shot duration. Values match the wire encoding directly (minutes, 0 = off).
enum class ShootingStarDuration : uint8_t {
  OFF = 0,
  ONE_MINUTE = 1,
  TWO_MINUTES = 2,
  FIVE_MINUTES = 5,
  TEN_MINUTES = 10,
};

struct RGBW {
  uint8_t r{0};
  uint8_t g{0};
  uint8_t b{0};
  uint8_t w{0};
};

namespace protocol {

// "T" / "F" / "B" / "R" / "P" / "J"
std::string starry_effect_command(StarryEffect effect);

// "w" / "f" / "r" / "b"
std::string color_transition_command(ColorTransition transition);

// "c,R,G,B,W*"
std::string starry_color_command(const RGBW &color);

// "D,{pct}*" — percent is clamped to the hardware's valid 10-100 range.
std::string star_dimmer_command(uint8_t percent);

// "{Zone},{R},{G},{B},{W},{time}*"
std::string shooting_star_command(ShootingStarZone zone, const RGBW &color, ShootingStarDuration duration);

// Convenience for the manual's explicit "turn zone off" case: "{Zone},0,0,0,0,0*"
std::string shooting_star_off_command(ShootingStarZone zone);

// "e,{speed}" / "k,{speed}" (no terminator) or "n" for NO_EFFECT (speed ignored).
// speed is clamped to the hardware's valid 150-255 range.
std::string ambient_effect_command(AmbientEffect effect, uint8_t speed);

// "A,R,G,B,W*"
std::string ambient_color_command(const RGBW &color);

// "d,{pct}*" — percent is clamped to the hardware's valid 10-100 range.
std::string ambient_dimmer_command(uint8_t percent);

}  // namespace protocol
}  // namespace illuminate
}  // namespace esphome
