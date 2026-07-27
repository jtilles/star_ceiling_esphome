// Serial command parser.
//
// Legacy grammar (byte-compatible with the original board and
// components/illuminate/illuminate_protocol.h - see docs/illuminate-
// firmware-notes.md's "RXD command handler" section for the confirmed wire
// format this was transcribed from):
//   'T'/'F'/'B'/'R'/'P'/'J'        - starry effect select (immediate, no terminator)
//   'w'/'f'/'r'/'b'                - color transition select (immediate, no terminator)
//   'c',R,G,B,W*                   - set starry color
//   'D',pct*                       - star dimmer (10-100)
//   'X'|'Y'|'Z',R,G,B,W,interval_s* - shooting star zone: fires one streak,
//                                    then one more every interval_s seconds
//                                    (0-65535) until turned off with
//                                    interval_s=0. Diverges from
//                                    illuminate_protocol.h's
//                                    ShootingStarDuration (which sends
//                                    0/1/2/5/10 meaning *minutes of
//                                    continuous firing*) based on real
//                                    bench-test feedback - see
//                                    shooting_star.h and
//                                    firmware/star_zone/README.md.
//
// New enhancement grammar, namespaced under a leading '!' so it can never
// collide with the legacy grammar above (which only ever starts a command
// with a bare letter) - see docs/illuminate-enhanced-prd.md's "Protocol
// versioning / extensibility strategy":
//   '!M',pct*                      - master brightness (0-100)
//   '!S',speed*                    - effect speed (0-255)
//   '!O',{0|1}[,transition_ms]*    - power on/off, optional fade duration
//   '!I',{0|1}*                    - identify/install mode on/off
//   '!N'*                          - identify: step to next LED
//   '!V'*                          - identify: step to previous LED
//   '!C',palette_id*               - palette-driven twinkle (0 = off/solid color)
//   '!W',{0|1}[,avg_interval_s]*   - meteor-shower auto mode on/off
//   '!L',zone,trail[,speed]*       - per-zone shooting star trail (pixels,
//                                    clamped 1-40) and optional speed
//                                    override (0-255; omit, or send 256+,
//                                    to fall back to the shared '!S' speed)
//   '!F',ms*                       - Wipe/Fade color transition duration
//                                    (50-10000ms, clamped)
//
// The undocumented 'O' single-byte command the original firmware's
// dispatcher accepts (docs/illuminate-firmware-notes.md's "Undocumented"
// table row) is intentionally left unhandled here - its behavior was never
// confirmed on real hardware, so there's nothing to replicate.
#pragma once

#include <Arduino.h>

#include "controller.h"

namespace star_zone {
namespace protocol {

// Reads any bytes currently available on `serial` and dispatches complete
// commands into `controller`. Non-blocking (never waits for more bytes than
// are already buffered by the UART) - call once per loop() iteration.
void poll(Stream &serial, StarZoneController &controller);

}  // namespace protocol
}  // namespace star_zone
