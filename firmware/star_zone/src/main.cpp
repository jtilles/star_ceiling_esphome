// Entry point for the Top-Star / Bot-Star replacement firmware. See
// firmware/star_zone/README.md (and the main repo README's firmware
// section) for what this is, what it assumes, and what still needs bench
// validation before trusting it on real hardware.
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "config.h"
#include "controller.h"
#include "protocol.h"

namespace {
Adafruit_NeoPixel strip(star_zone::kLedCount, star_zone::kLedDataPin, star_zone::kLedColorOrder);
star_zone::StarZoneController controller(strip);

// Render cadence, and why it exists: Adafruit_NeoPixel::show() disables
// interrupts for the entire pixel transmission - 512 bits x 1.25us = ~640us
// for this strip's 16 RGBW pixels. Calling it every loop() iteration (which
// is what this firmware originally did) keeps interrupts off well over half
// the time, and the UART RX interrupt can't run while they are, so incoming
// command bytes are simply lost. Rendering on a fixed ~50 FPS timer instead
// drops that duty cycle to ~3% and leaves serial reception alone; 20ms is
// still far faster than any of the effects animate.
//
// This is the actual cause of what the README long recorded as an unexplained
// hardware quirk ("Top-Star reception of longer commands" - single-byte
// commands working while multi-byte 'c,R,G,B,W*' ones silently did nothing).
// It was never signal integrity on one chip: a single-byte command only has
// to survive one blackout window, while a 12-byte command has to survive
// twelve. Expanding the strip from 8 to 16 LEDs doubled show()'s blackout and
// pushed it from flaky to totally non-functional - confirmed on real hardware
// by bisecting LED output against serial input with standalone test firmware.
constexpr uint32_t kRenderIntervalMs = 20;
uint32_t last_render_ms = 0;
}  // namespace

void setup() {
  Serial.begin(star_zone::kSerialBaud);
  controller.begin();
}

void loop() {
  // Poll serial every iteration (cheap, non-blocking); render on the timer.
  star_zone::protocol::poll(Serial, controller);
  uint32_t now = millis();
  if (now - last_render_ms >= kRenderIntervalMs) {
    last_render_ms = now;
    controller.tick(now);
  }
}
