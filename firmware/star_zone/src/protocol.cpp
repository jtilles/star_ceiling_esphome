#include "protocol.h"

#include <ctype.h>
#include <stdlib.h>

#include "config.h"

namespace star_zone {
namespace protocol {

namespace {

char buf[kCommandBufferSize];
uint8_t buf_len = 0;

bool immediate_effect(char c, StarryEffect &out) {
  switch (c) {
    case 'T':
      out = StarryEffect::TWINKLE;
      return true;
    case 'F':
      out = StarryEffect::FIREFLY;
      return true;
    case 'B':
      out = StarryEffect::BREEZE;
      return true;
    case 'R':
      out = StarryEffect::RANDOM;
      return true;
    case 'P':
      out = StarryEffect::RAIN_DROPS;
      return true;
    case 'J':
      out = StarryEffect::JAMBOREE;
      return true;
    default:
      return false;
  }
}

bool immediate_transition(char c, ColorTransition &out) {
  switch (c) {
    case 'w':
      out = ColorTransition::WIPE;
      return true;
    case 'f':
      out = ColorTransition::FADE;
      return true;
    case 'r':
      out = ColorTransition::RAINBOW;
      return true;
    case 'b':
      out = ColorTransition::BLINK;
      return true;
    default:
      return false;
  }
}

// Whether a byte starts a buffered (comma-delimited, '*'-terminated) legacy
// command.
bool starts_buffered_legacy(char c) { return c == 'c' || c == 'D' || c == 'X' || c == 'Y' || c == 'Z'; }

// Parses the next unsigned integer field at *p, advancing *p past the
// following ',' or '*'. Returns false (leaving *p unmoved) if *p isn't a
// digit.
bool read_field(const char *&p, uint16_t &out) {
  if (!isdigit((unsigned char) *p)) return false;
  char *end;
  out = (uint16_t) strtoul(p, &end, 10);
  p = end;
  if (*p == ',' || *p == '*') p++;
  return true;
}

void dispatch_legacy(const char *cmd, StarZoneController &controller) {
  char head = cmd[0];
  const char *p = cmd + 1;
  if (*p == ',') p++;
  uint16_t f[5];

  switch (head) {
    case 'c':
      if (!read_field(p, f[0]) || !read_field(p, f[1]) || !read_field(p, f[2]) || !read_field(p, f[3])) return;
      controller.set_color(RGBW{(uint8_t) f[0], (uint8_t) f[1], (uint8_t) f[2], (uint8_t) f[3]});
      return;
    case 'D':
      if (!read_field(p, f[0])) return;
      controller.set_dimmer((uint8_t) f[0]);
      return;
    case 'X':
    case 'Y':
    case 'Z': {
      if (!read_field(p, f[0]) || !read_field(p, f[1]) || !read_field(p, f[2]) || !read_field(p, f[3]) ||
          !read_field(p, f[4]))
        return;
      ShootingStarZone zone =
          head == 'X' ? ShootingStarZone::ZONE_X : (head == 'Y' ? ShootingStarZone::ZONE_Y : ShootingStarZone::ZONE_Z);
      controller.set_shooting_star(zone, RGBW{(uint8_t) f[0], (uint8_t) f[1], (uint8_t) f[2], (uint8_t) f[3]},
                                    (uint8_t) f[4]);
      return;
    }
    default:
      return;
  }
}

void dispatch_enhancement(const char *cmd, uint8_t len, StarZoneController &controller) {
  if (len < 2) return;
  char sub = cmd[1];
  const char *p = cmd + 2;
  if (*p == ',') p++;
  uint16_t f[2];

  switch (sub) {
    case 'M':
      if (read_field(p, f[0])) controller.set_master_brightness((uint8_t) f[0]);
      return;
    case 'S':
      if (read_field(p, f[0])) controller.set_effect_speed((uint8_t) f[0]);
      return;
    case 'O':
      if (read_field(p, f[0])) {
        uint16_t transition_ms = 500;
        read_field(p, transition_ms);  // optional - default kept on failure
        controller.set_power(f[0] != 0, transition_ms);
      }
      return;
    case 'I':
      if (read_field(p, f[0])) controller.set_identify_mode(f[0] != 0);
      return;
    case 'N':
      controller.identify_step(1);
      return;
    case 'V':
      controller.identify_step(-1);
      return;
    case 'C':
      if (read_field(p, f[0])) controller.set_palette((uint8_t) f[0]);
      return;
    case 'W':
      if (read_field(p, f[0])) {
        uint16_t interval_s = 20;
        read_field(p, interval_s);  // optional - default kept on failure
        controller.set_meteor_shower(f[0] != 0, interval_s);
      }
      return;
    default:
      return;
  }
}

}  // namespace

void poll(Stream &serial, StarZoneController &controller) {
  while (serial.available() > 0) {
    char c = (char) serial.read();

    if (buf_len == 0) {
      StarryEffect effect;
      ColorTransition transition;
      if (immediate_effect(c, effect)) {
        controller.set_effect(effect);
      } else if (immediate_transition(c, transition)) {
        controller.set_transition(transition);
      } else if (starts_buffered_legacy(c) || c == '!') {
        buf[buf_len++] = c;
      }
      // Anything else - including the original firmware's undocumented 'O'
      // byte, see protocol.h - is silently dropped, matching how the
      // original dispatcher ignores bytes outside its table.
      continue;
    }

    if (buf_len < kCommandBufferSize - 1) buf[buf_len++] = c;

    if (c == '*') {
      buf[buf_len] = '\0';
      if (buf[0] == '!') {
        dispatch_enhancement(buf, buf_len, controller);
      } else {
        dispatch_legacy(buf, controller);
      }
      buf_len = 0;
    } else if (buf_len >= kCommandBufferSize - 1) {
      buf_len = 0;  // overflow guard: drop and resync on garbage/oversized input
    }
  }
}

}  // namespace protocol
}  // namespace star_zone
