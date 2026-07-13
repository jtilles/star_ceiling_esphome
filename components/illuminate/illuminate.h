// IlluminateHub: ESPHome plumbing (Component lifecycle, UARTDevice) for the Fusion
// Synergy "Illuminate 1.0" controller. Deliberately thin — all command-string
// construction lives in illuminate_protocol.h/.cpp; this class only owns the UART bus
// and forwards pre-built command strings to it.
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

  void send_shooting_star(ShootingStarZone zone, const RGBW &color, ShootingStarDuration duration);
  void send_shooting_star_off(ShootingStarZone zone);

  void send_ambient_effect(AmbientEffect effect, uint8_t speed);
  void send_ambient_color(const RGBW &color);
  void send_ambient_dimmer(uint8_t percent);

 protected:
  void send_command_(const std::string &command);
};

}  // namespace illuminate
}  // namespace esphome
