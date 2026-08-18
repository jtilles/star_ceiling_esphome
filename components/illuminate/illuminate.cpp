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

void IlluminateHub::send_shooting_star(ShootingStarZone zone, const RGBW &color, uint16_t interval_seconds) {
  this->send_command_(protocol::shooting_star_command(zone, color, interval_seconds));
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

void IlluminateHub::send_master_brightness(uint8_t percent) {
  this->send_command_(protocol::master_brightness_command(percent));
}

void IlluminateHub::send_shooting_star_brightness(uint8_t percent) {
  this->send_command_(protocol::shooting_star_brightness_command(percent));
}

void IlluminateHub::send_effect_speed(uint8_t speed) { this->send_command_(protocol::effect_speed_command(speed)); }

void IlluminateHub::send_power(bool on, uint16_t transition_ms) {
  this->send_command_(protocol::power_command(on, transition_ms));
}

void IlluminateHub::send_identify(bool enabled) { this->send_command_(protocol::identify_command(enabled)); }

void IlluminateHub::send_identify_next() { this->send_command_(protocol::identify_next_command()); }

void IlluminateHub::send_identify_previous() { this->send_command_(protocol::identify_previous_command()); }

void IlluminateHub::send_palette(StarPalette palette) { this->send_command_(protocol::palette_command(palette)); }

void IlluminateHub::send_meteor_shower(bool on, uint16_t avg_interval_seconds) {
  this->send_command_(protocol::meteor_shower_command(on, avg_interval_seconds));
}

void IlluminateHub::send_shooting_star_tuning(ShootingStarZone zone, uint8_t trail, uint16_t speed, uint16_t decay) {
  this->send_command_(protocol::shooting_star_tuning_command(zone, trail, speed, decay));
}

void IlluminateHub::send_color_transition_duration(uint16_t milliseconds) {
  this->send_command_(protocol::color_transition_duration_command(milliseconds));
}

}  // namespace illuminate
}  // namespace esphome
