#include "illuminate.h"
#include "esphome/core/log.h"

namespace esphome {
namespace illuminate {

static const char *const TAG = "illuminate";

void IlluminateHub::setup() {}

void IlluminateHub::dump_config() { ESP_LOGCONFIG(TAG, "Illuminate Hub:"); }

void IlluminateHub::send_command_(const std::string &command) { this->write_str(command.c_str()); }

void IlluminateHub::send_starry_effect(StarryEffect effect) {
  this->send_command_(protocol::starry_effect_command(effect));
}

void IlluminateHub::send_color_transition(ColorTransition transition) {
  this->send_command_(protocol::color_transition_command(transition));
}

void IlluminateHub::send_starry_color(const RGBW &color) { this->send_command_(protocol::starry_color_command(color)); }

void IlluminateHub::send_star_dimmer(uint8_t percent) { this->send_command_(protocol::star_dimmer_command(percent)); }

void IlluminateHub::send_shooting_star(ShootingStarZone zone, const RGBW &color, ShootingStarDuration duration) {
  this->send_command_(protocol::shooting_star_command(zone, color, duration));
}

void IlluminateHub::send_shooting_star_off(ShootingStarZone zone) {
  this->send_command_(protocol::shooting_star_off_command(zone));
}

void IlluminateHub::send_ambient_effect(AmbientEffect effect, uint8_t speed) {
  this->send_command_(protocol::ambient_effect_command(effect, speed));
}

void IlluminateHub::send_ambient_color(const RGBW &color) { this->send_command_(protocol::ambient_color_command(color)); }

void IlluminateHub::send_ambient_dimmer(uint8_t percent) {
  this->send_command_(protocol::ambient_dimmer_command(percent));
}

}  // namespace illuminate
}  // namespace esphome
