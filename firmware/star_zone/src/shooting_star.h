// Shooting Star: genuinely new functionality on this firmware, not a port.
//
// The original Top-Star/Bot-Star firmware has zero Shooting Star support -
// confirmed by decompilation, see docs/illuminate-firmware-notes.md's
// "Shooting Star is absent from this firmware" section. There is no
// original behavior to replicate, so this is an original design.
//
// The wire command shape - "{Zone},{R},{G},{B},{W},{value}*" - matches
// components/illuminate/illuminate_protocol.h's shooting_star_command so the
// existing ESPHome hub/light platform's command *framing* still parses
// unmodified. The meaning of the trailing numeric field has diverged from
// that component's ShootingStarDuration enum (which sends 0/1/2/5/10 meaning
// minutes of continuous firing) based on real bench-test feedback: a fixed
// "run continuously for N minutes then stop" mode wasn't useful. Here, that
// field is `interval_seconds` (0-65535, 0 = off) - the zone fires one streak,
// then automatically fires another every `interval_seconds`, indefinitely,
// until turned off with interval_seconds=0. See firmware/star_zone/README.md
// for the full rationale and range table.
//
// Each zone (X/Y/Z) is an independent "slot" sharing one physical 16-LED
// strip (not a merged Top+Bottom array) - matching the PRD's "simultaneous
// multi-streak support" enhancement, scoped down to the 3 named zones.
#pragma once

#include "config.h"
#include "types.h"

namespace star_zone {

struct ShootingStarSlot {
  bool firing{false};
  bool single_shot{false};  // true for meteor-shower one-off shots (see controller.cpp)
  RGBW color{};
  uint32_t interval_ms{0};    // ms between automatic streak launches (0 only valid with single_shot)
  uint32_t next_streak_ms{0};
  bool streak_active{false};
  uint32_t streak_start_ms{0};

  // Per-zone tuning, set via the "!L" enhancement command
  // (StarZoneController::set_shooting_star_config) - defaults reproduce the
  // fixed values this firmware originally hardcoded, so a zone that's never
  // been configured behaves exactly as before.
  float trail_pixels{3.0f};        // fade length behind the streak's head, in pixels
  int16_t speed_override{-1};      // -1 = fall back to the shared "!S" effect speed
  // Fade-curve exponent applied to the linear head-to-tail brightness ramp
  // (see shooting_star.cpp's tick()). 1.0 = pure linear, this firmware's
  // original hardcoded shape, where trail_pixels alone controlled both the
  // tail's visual length *and* its falloff steepness (steepness was always
  // exactly 255/trail_pixels). This decouples the two: >1.0 dims faster
  // near the head (shorter-looking glow for the same trail_pixels), <1.0
  // stays brighter for longer before dropping off near the very end.
  float decay_exponent{1.0f};
};

namespace shooting_star {

// Starts firing one streak every `interval_seconds` (0-65535), indefinitely,
// or stops the zone immediately (interval_seconds == 0, matching
// illuminate_protocol::shooting_star_off_command's "{Zone},0,0,0,0,0*"
// shorthand - still valid, "0" still means off). `single_shot` is for
// internal use by the meteor-shower enhancement (see
// StarZoneController::maybe_fire_meteor_) - it fires exactly one streak and
// then clears `firing` on its own, ignoring `interval_seconds` entirely.
void start(ShootingStarSlot &slot, const RGBW &color, uint16_t interval_seconds, uint32_t now_ms,
           bool single_shot = false);

// Advances one slot's animation and additively blends any in-flight streak
// onto `pixels` (color_utils::blend_add, so overlapping zones brighten
// rather than overwrite each other). Returns true while the zone is still
// firing (including the gap between streaks), false once it's fully done.
//
// `brightness_pct` (0-100) scales the streak independently of the Starry
// Night layer underneath it - see StarZoneController::render_ and the "!B"
// command. Folded into the per-pixel fade math rather than applied to the
// buffer afterwards, so it costs no extra pixel buffer.
bool tick(ShootingStarSlot &slot, RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed,
          uint8_t brightness_pct = 100);

}  // namespace shooting_star
}  // namespace star_zone
