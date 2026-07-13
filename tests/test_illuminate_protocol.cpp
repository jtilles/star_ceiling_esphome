// Lightweight assertion-based tests for the pure protocol layer, independent of ESPHome
// and Arduino entirely. Build and run with a normal desktop g++/clang++:
//
//   g++ -std=c++17 -Wall -Wextra \
//       -I ../components/illuminate \
//       test_illuminate_protocol.cpp ../components/illuminate/illuminate_protocol.cpp \
//       -o test_illuminate_protocol
//   ./test_illuminate_protocol
//
// Focused on the cases most likely to silently regress against the manual: shooting star
// zone/time encoding, the "turn zone off" shorthand, and ambient effect's no-terminator
// quirk vs. the starred (*) commands used everywhere else.
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

  // Shooting star: {Zone},{R},{G},{B},{W},{time}*
  assert(shooting_star_command(ShootingStarZone::ZONE_1, RGBW{255, 0, 0, 0}, ShootingStarDuration::ONE_MINUTE) ==
         "X,255,0,0,0,1*");
  assert(shooting_star_command(ShootingStarZone::ZONE_2, RGBW{0, 0, 255, 0}, ShootingStarDuration::TEN_MINUTES) ==
         "Y,0,0,255,0,10*");
  assert(shooting_star_command(ShootingStarZone::ZONE_3, RGBW{0, 255, 0, 0}, ShootingStarDuration::FIVE_MINUTES) ==
         "Z,0,255,0,0,5*");

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

  // Ambient color: A,R,G,B,W*
  assert(ambient_color_command(RGBW{255, 0, 0, 0}) == "A,255,0,0,0*");

  // Ambient/strip dimmer: d,{pct}*
  assert(ambient_dimmer_command(80) == "d,80*");
  assert(ambient_dimmer_command(200) == "d,100*");

  std::printf("All illuminate_protocol tests passed.\n");
  return 0;
}
