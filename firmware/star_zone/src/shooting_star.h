// Shooting Star: genuinely new functionality on this firmware, not a port.
//
// The original Top-Star/Bot-Star firmware has zero Shooting Star support -
// confirmed by decompilation, see docs/illuminate-firmware-notes.md's
// "Shooting Star is absent from this firmware" section. There is no
// original behavior to replicate, so this is an original design built to
// satisfy the wire-compatible command shape already defined in
// components/illuminate/illuminate_protocol.h
// (shooting_star_command: "{Zone},{R},{G},{B},{W},{time}*", where time is
// 0/1/2/5/10 minutes) so the existing ESPHome hub/light platform work
// against it unmodified.
//
// Each zone (X/Y/Z) is an independent "slot" that, once started, fires
// repeating meteor streaks across this chip's LED strip until its duration
// elapses (or it's cancelled with time=0) - matching the PRD's "simultaneous
// multi-streak support" enhancement, scoped down to the 3 named zones
// sharing one physical 16-LED strip instead of a merged Top+Bottom array.
#pragma once

#include "config.h"
#include "types.h"

namespace star_zone {

struct ShootingStarSlot {
  bool firing{false};
  bool single_shot{false};  // true for meteor-shower one-off shots (see controller.cpp)
  RGBW color{};
  uint32_t stop_at_ms{0};
  uint32_t next_streak_ms{0};
  bool streak_active{false};
  uint32_t streak_start_ms{0};

  // Per-zone tuning, set via the "!L" enhancement command
  // (StarZoneController::set_shooting_star_config) - defaults reproduce the
  // fixed values this firmware originally hardcoded, so a zone that's never
  // been configured behaves exactly as before.
  float trail_pixels{3.0f};        // fade length behind the streak's head, in pixels
  int16_t speed_override{-1};      // -1 = fall back to the shared "!S" effect speed
};

namespace shooting_star {

// Starts repeating streaks (duration_minutes 1/2/5/10), or stops the zone
// immediately (duration_minutes == 0, matching
// illuminate_protocol::shooting_star_off_command's "{Zone},0,0,0,0,0*"
// shorthand). `single_shot` is for internal use by the meteor-shower
// enhancement (see StarZoneController::maybe_fire_meteor_) - it fires
// exactly one streak and then clears `firing` on its own.
void start(ShootingStarSlot &slot, const RGBW &color, uint8_t duration_minutes, uint32_t now_ms,
           bool single_shot = false);

// Advances one slot's animation and additively blends any in-flight streak
// onto `pixels` (color_utils::blend_add, so overlapping zones brighten
// rather than overwrite each other). Returns true while the zone is still
// firing (including the gap between streaks), false once it's fully done.
bool tick(ShootingStarSlot &slot, RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed);

}  // namespace shooting_star
}  // namespace star_zone
