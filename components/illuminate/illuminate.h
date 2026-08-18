// IlluminateHub: ESPHome plumbing (Component lifecycle, UARTDevice) for the Fusion
// Synergy "Illuminate 1.0" controller. Deliberately thin - all command-string
// construction lives in illuminate_protocol.h/.cpp; this class only owns the UART bus
// and forwards pre-built command strings to it.
//
// The send_* methods below are public so they can be reached from YAML lambdas
// (`id(illuminate_hub).send_master_brightness(x);`) as well as from this component's own
// platforms - that is how the enhancement commands are surfaced as HA entities without
// each one needing its own codegen platform. See cooper_ceiling.yaml.
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "illuminate_protocol.h"

namespace esphome {
namespace illuminate {

class IlluminateHub : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void send_starry_effect(StarryEffect effect);
  void send_color_transition(ColorTransition transition);
  void send_starry_color(const RGBW &color);
  void send_star_dimmer(uint8_t percent);

  void send_shooting_star(ShootingStarZone zone, const RGBW &color, uint16_t interval_seconds);
  void send_shooting_star_off(ShootingStarZone zone);

  void send_ambient_effect(AmbientEffect effect, uint8_t speed);
  void send_ambient_color(const RGBW &color);
  void send_ambient_dimmer(uint8_t percent);

  // Enhancement commands - star_zone replacement firmware only. These are silently
  // ignored by stock zone firmware, so it is harmless to call them on a stock board;
  // they simply do nothing.
  void send_master_brightness(uint8_t percent);
  void send_shooting_star_brightness(uint8_t percent);
  void send_effect_speed(uint8_t speed);
  void send_power(bool on, uint16_t transition_ms = 500);
  void send_identify(bool enabled);
  void send_identify_next();
  void send_identify_previous();
  void send_palette(StarPalette palette);
  void send_meteor_shower(bool on, uint16_t avg_interval_seconds = 20);
  void send_shooting_star_tuning(ShootingStarZone zone, uint8_t trail, uint16_t speed, uint16_t decay);
  void send_color_transition_duration(uint16_t milliseconds);

 protected:
  void send_command_(const std::string &command);
};

}  // namespace illuminate
}  // namespace esphome
