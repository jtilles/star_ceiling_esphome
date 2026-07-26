// Shared value types for the Top-Star / Bot-Star replacement firmware.
//
// Mirrors the enums in components/illuminate/illuminate_protocol.h so the
// legacy command surface stays obviously in sync with the ESPHome side, but
// this header has no ESPHome/std::string dependency - AVR builds avoid
// std::string entirely (see protocol.h).
#pragma once

#include <stdint.h>

namespace star_zone {

struct RGBW {
  uint8_t r{0};
  uint8_t g{0};
  uint8_t b{0};
  uint8_t w{0};

  // Explicit constructor rather than relying on aggregate-init: this
  // toolchain's default C++ standard (gnu++11) doesn't treat a struct with
  // default member initializers as an aggregate, so `RGBW{1,2,3,4}` needs a
  // real constructor to resolve.
  RGBW() = default;
  RGBW(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t w_ = 0) : r(r_), g(g_), b(b_), w(w_) {}
};

// Starry Night animation effects. Matches
// components/illuminate/illuminate_protocol.h's StarryEffect byte-for-byte
// (T/F/B/R/P/J) - see protocol.cpp for the wire mapping.
enum class StarryEffect : uint8_t {
  TWINKLE,
  FIREFLY,
  BREEZE,
  RANDOM,
  RAIN_DROPS,
  JAMBOREE,
};

// Color transitions (w/f/r/b). NONE is a firmware-internal default (static
// color, no continuous modifier) - the original protocol has no explicit
// "none" command, it's just what a chip does before its first w/f/r/b.
enum class ColorTransition : uint8_t {
  NONE,
  WIPE,
  FADE,
  RAINBOW,
  BLINK,
};

// Shooting Star zones (X/Y/Z, "SS1"/"SS2"/"SS3" in the manual). On this
// standalone zone chip all three are virtual slots on the same physical
// LED strip (docs/illuminate-enhanced-pinmap.md's "virtual zone" concept,
// applied per-chip instead of across a merged array) - see shooting_star.h.
enum class ShootingStarZone : uint8_t {
  ZONE_X = 0,
  ZONE_Y = 1,
  ZONE_Z = 2,
};
constexpr uint8_t kShootingStarZoneCount = 3;

}  // namespace star_zone
