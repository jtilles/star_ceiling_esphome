// Lightweight assertion-based tests for the pure protocol layer, independent of ESPHome
// and Arduino entirely. Build and run with a normal desktop g++/clang++:
//
//   g++ -std=c++17 -Wall -Wextra \
//       -I ../components/illuminate \
//       test_illuminate_protocol.cpp ../components/illuminate/illuminate_protocol.cpp \
//       -o test_illuminate_protocol
//   ./test_illuminate_protocol
//
// Focused on the cases most likely to silently regress: shooting star zone/interval
// encoding, the "turn zone off" shorthand, ambient effect's no-terminator quirk vs. the
// starred (*) commands used everywhere else, and the clamp ranges on the '!' enhancement
// commands (which are documented per-field in firmware/star_zone/README.md - if a clamp
// here and a clamp there disagree, the firmware is the authority).
#include <cassert>
#include <cstdio>

#include "illuminate_protocol.h"

using namespace esphome::illuminate;
using namespace esphome::illuminate::protocol;

int main() {
  // Starry Night effects: single character, no terminator.
  assert(starry_effect_command(StarryEffect::TWINKLE) == "T");
  assert(starry_effect_command(StarryEffect::JAMBOREE) == "J");

  // Color transitions: single character, no terminator.
  assert(color_transition_command(ColorTransition::WIPE) == "w");
  assert(color_transition_command(ColorTransition::RAINBOW) == "r");

  // Starry Night color: c,R,G,B,W*
  assert(starry_color_command(RGBW{255, 0, 0, 0}) == "c,255,0,0,0*");
  assert(starry_color_command(RGBW{0, 0, 0, 255}) == "c,0,0,0,255*");  // White

  // Star dimmer: D,{pct}* with 10-100 clamp (hardware floor is 10, not 0).
  assert(star_dimmer_command(80) == "D,80*");
  assert(star_dimmer_command(0) == "D,10*");    // below floor clamps up
  assert(star_dimmer_command(5) == "D,10*");    // below floor clamps up
  assert(star_dimmer_command(150) == "D,100*"); // above ceiling clamps down

  // Shooting star: {Zone},{R},{G},{B},{W},{interval_s}* - the trailing field is SECONDS
  // BETWEEN shots (star_zone firmware), not the manual's minutes of continuous firing.
  assert(shooting_star_command(ShootingStarZone::ZONE_1, RGBW{255, 0, 0, 0}, 1) == "X,255,0,0,0,1*");
  assert(shooting_star_command(ShootingStarZone::ZONE_2, RGBW{0, 0, 255, 0}, 10) == "Y,0,0,255,0,10*");
  assert(shooting_star_command(ShootingStarZone::ZONE_3, RGBW{0, 255, 0, 0}, 45) == "Z,0,255,0,0,45*");
  // Full 16-bit range is valid and must not be truncated to a byte.
  assert(shooting_star_command(ShootingStarZone::ZONE_1, RGBW{1, 2, 3, 4}, 65535) == "X,1,2,3,4,65535*");
  assert(shooting_star_command(ShootingStarZone::ZONE_1, RGBW{1, 2, 3, 4}, 300) == "X,1,2,3,4,300*");

  // Explicit "off" shorthand from the manual: X,0,0,0,0,0*
  assert(shooting_star_off_command(ShootingStarZone::ZONE_1) == "X,0,0,0,0,0*");
  assert(shooting_star_off_command(ShootingStarZone::ZONE_2) == "Y,0,0,0,0,0*");

  // Ambient effects: e,{speed} / k,{speed} have NO trailing '*' (unlike almost everything
  // else in the protocol), and "n" takes no speed argument at all.
  assert(ambient_effect_command(AmbientEffect::FADE, 200) == "e,200");
  assert(ambient_effect_command(AmbientEffect::BLINK, 255) == "k,255");
  assert(ambient_effect_command(AmbientEffect::NO_EFFECT, 200) == "n");
  assert(ambient_effect_command(AmbientEffect::FADE, 100) == "e,150");  // below floor clamps up
  assert(ambient_effect_command(AmbientEffect::BLINK, 255) == "k,255"); // at ceiling

  // --- Enhancement commands (star_zone replacement firmware only) ------------

  // !M / !B: 0-100, and unlike the D/d dimmers a true 0 is valid (not floored to 10).
  assert(master_brightness_command(75) == "!M,75*");
  assert(master_brightness_command(0) == "!M,0*");
  assert(master_brightness_command(200) == "!M,100*");
  assert(shooting_star_brightness_command(0) == "!B,0*");
  assert(shooting_star_brightness_command(101) == "!B,100*");

  // !S: full 0-255 byte, no clamping - each effect maps it onto its own period range.
  assert(effect_speed_command(0) == "!S,0*");
  assert(effect_speed_command(255) == "!S,255*");

  // !O: power with an explicit fade duration.
  assert(power_command(true, 500) == "!O,1,500*");
  assert(power_command(false, 0) == "!O,0,0*");

  // !I / !N / !V: identify mode and its LED stepping.
  assert(identify_command(true) == "!I,1*");
  assert(identify_command(false) == "!I,0*");
  assert(identify_next_command() == "!N*");
  assert(identify_previous_command() == "!V*");

  // !C: palette id, with out-of-range ids clamped to the top palette.
  assert(palette_command(StarPalette::NONE) == "!C,0*");
  assert(palette_command(StarPalette::FIRE) == "!C,1*");
  assert(palette_command(StarPalette::RAINBOW_CYCLE) == "!C,5*");

  // !W: meteor shower; 0 would be a divide-by-zero in the firmware, so it floors to 1.
  assert(meteor_shower_command(true, 20) == "!W,1,20*");
  assert(meteor_shower_command(true, 0) == "!W,1,1*");
  assert(meteor_shower_command(false, 20) == "!W,0,20*");

  // !L: per-zone tuning. All four fields are always emitted because the firmware treats
  // an omitted trailing field as a RESET, not as "leave unchanged".
  assert(shooting_star_tuning_command(ShootingStarZone::ZONE_1, 8, 200, 100) == "!L,X,8,200,100*");
  assert(shooting_star_tuning_command(ShootingStarZone::ZONE_2, 0, 0, 0) == "!L,Y,1,0,10*");     // clamps up
  assert(shooting_star_tuning_command(ShootingStarZone::ZONE_3, 99, 255, 900) == "!L,Z,40,255,500*");  // clamps down
  // 256+ in the speed field is the firmware's "clear this zone's override" sentinel, so
  // it must pass through as 256 rather than being clamped into the 0-255 byte range.
  assert(shooting_star_tuning_command(ShootingStarZone::ZONE_1, 3, protocol::SHOOTING_STAR_SPEED_INHERIT, 100) ==
         "!L,X,3,256,100*");
  assert(shooting_star_tuning_command(ShootingStarZone::ZONE_1, 3, 4000, 100) == "!L,X,3,256,100*");

  // !F: wipe/fade transition duration, clamped 50-10000ms.
  assert(color_transition_duration_command(700) == "!F,700*");
  assert(color_transition_duration_command(10) == "!F,50*");
  assert(color_transition_duration_command(60000) == "!F,10000*");

  // Ambient color: A,R,G,B,W*
  assert(ambient_color_command(RGBW{255, 0, 0, 0}) == "A,255,0,0,0*");

  // Ambient/strip dimmer: d,{pct}*
  assert(ambient_dimmer_command(80) == "d,80*");
  assert(ambient_dimmer_command(200) == "d,100*");

  std::printf("All illuminate_protocol tests passed.\n");
  return 0;
}
