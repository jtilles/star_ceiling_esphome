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
}  // namespace

void setup() {
  Serial.begin(star_zone::kSerialBaud);
  controller.begin();
}

void loop() {
  star_zone::protocol::poll(Serial, controller);
  controller.tick(millis());
}
