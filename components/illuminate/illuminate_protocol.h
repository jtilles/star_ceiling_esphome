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
//
// The `!`-prefixed "enhancement" commands at the bottom are NOT part of the original
// manual - they are implemented only by this repo's replacement zone firmware
// (firmware/star_zone/, see its README for every field's range and rationale). They are
// namespaced under a leading '!' precisely so they can never collide with the legacy
// grammar above, and stock zone chips ignore them.
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

// Shooting Star zones. Independent, concurrent slots on star_zone firmware.
enum class ShootingStarZone : uint8_t {
  ZONE_1,  // "X" / SS1
  ZONE_2,  // "Y" / SS2
  ZONE_3,  // "Z" / SS3
};

// Built-in palettes for `!C` (star_zone firmware only). Anything above RAINBOW_CYCLE is
// treated as NONE by the firmware rather than erroring; the clamp here matches.
enum class StarPalette : uint8_t {
  NONE = 0,
  FIRE = 1,
  OCEAN = 2,
  FOREST = 3,
  CANDY = 4,
  RAINBOW_CYCLE = 5,
};

struct RGBW {
  uint8_t r{0};
  uint8_t g{0};
  uint8_t b{0};
  uint8_t w{0};
};

namespace protocol {

// Sentinel for shooting_star_tuning_command()'s `speed` field: the firmware clears a
// zone's per-zone speed override (falling back to the shared `!S` speed) when the field
// is 256 or higher, which is why this is a uint16_t rather than a uint8_t.
constexpr uint16_t SHOOTING_STAR_SPEED_INHERIT = 256;

// "T" / "F" / "B" / "R" / "P" / "J"
std::string starry_effect_command(StarryEffect effect);

// "w" / "f" / "r" / "b"
std::string color_transition_command(ColorTransition transition);

// "c,R,G,B,W*"
std::string starry_color_command(const RGBW &color);

// "D,{pct}*" - percent is clamped to the hardware's valid 10-100 range.
//
// NOTE: on star_zone firmware this scales the Starry Night layer ONLY, and is applied
// before shooting-star streaks are blended in - use shooting_star_brightness_command()
// for streaks and master_brightness_command() for the composited frame.
std::string star_dimmer_command(uint8_t percent);

// "{Zone},{R},{G},{B},{W},{interval_s}*"
//
// The trailing field is seconds BETWEEN shots, not a shot length: the zone fires one
// streak immediately, then another every `interval_seconds`, indefinitely, until it is
// sent 0. This is star_zone firmware's reinterpretation of the wire position the
// original manual documented as minutes-of-continuous-firing (0/1/2/5/10); the position
// is unchanged, its meaning is not. See firmware/star_zone/README.md's "Shooting star
// zone" entry. Range is the full 0-65535 the 16-bit field allows.
std::string shooting_star_command(ShootingStarZone zone, const RGBW &color, uint16_t interval_seconds);

// Convenience for the explicit "turn zone off" case: "{Zone},0,0,0,0,0*"
std::string shooting_star_off_command(ShootingStarZone zone);

// "e,{speed}" / "k,{speed}" (no terminator) or "n" for NO_EFFECT (speed ignored).
// speed is clamped to the hardware's valid 150-255 range.
std::string ambient_effect_command(AmbientEffect effect, uint8_t speed);

// "A,R,G,B,W*"
std::string ambient_color_command(const RGBW &color);

// "d,{pct}*" - percent is clamped to the hardware's valid 10-100 range.
std::string ambient_dimmer_command(uint8_t percent);

// ---------------------------------------------------------------------------
// Enhancement commands - star_zone replacement firmware only (see header note).
// ---------------------------------------------------------------------------

// "!M,{pct}*" - master brightness, clamped 0-100. Unlike the 10-floored star dimmer, 0
// is valid here: this is the control that can take the strip fully dark without
// disturbing the dimmer/color state underneath it.
std::string master_brightness_command(uint8_t percent);

// "!B,{pct}*" - shooting-star streak brightness, clamped 0-100, independent of "D".
std::string shooting_star_brightness_command(uint8_t percent);

// "!S,{speed}*" - shared effect/shooting-star speed, full 0-255 byte range (each effect
// maps the same byte onto its own period range; it is not linear milliseconds).
std::string effect_speed_command(uint8_t speed);

// "!O,{0|1},{transition_ms}*" - power off/on with a smooth fade. transition_ms is
// unclamped (any uint16_t is accepted by the firmware, up to ~65s).
std::string power_command(bool on, uint16_t transition_ms);

// "!I,{0|1}*" - identify/install mode: lights exactly one LED, suspends effect rendering.
std::string identify_command(bool enabled);

// "!N*" / "!V*" - step identify mode to the next/previous LED (wraps at strip ends).
std::string identify_next_command();
std::string identify_previous_command();

// "!C,{palette_id}*" - palette-driven twinkle; NONE reverts to the plain commanded color.
std::string palette_command(StarPalette palette);

// "!W,{0|1},{avg_interval_s}*" - meteor-shower auto mode. Timing is randomly jittered
// around avg_interval_s (floored at 1; the firmware treats 0 as 1 rather than dividing
// by zero), which is what distinguishes it from a fast fixed-cadence X/Y/Z zone.
std::string meteor_shower_command(bool on, uint16_t avg_interval_seconds);

// "!L,{zone},{trail},{speed},{decay}*" - per-zone shooting star tuning.
//
// `trail` (pixels of fade behind the head) clamps to 1-40. `speed` overrides the shared
// "!S" speed for this zone only; pass SHOOTING_STAR_SPEED_INHERIT to clear the override.
// `decay` is a fade-curve exponent in hundredths, clamped 10-500 (i.e. 0.10-5.00), where
// 100 is the linear falloff this firmware originally hardcoded.
//
// All four fields are always emitted because "!L" fully re-specifies a zone's tuning
// rather than patching individual fields - omitting a trailing field RESETS it.
std::string shooting_star_tuning_command(ShootingStarZone zone, uint8_t trail, uint16_t speed, uint16_t decay);

// "!F,{ms}*" - Wipe/Fade color-transition duration, clamped 50-10000ms.
std::string color_transition_duration_command(uint16_t milliseconds);

}  // namespace protocol
}  // namespace illuminate
}  // namespace esphome
