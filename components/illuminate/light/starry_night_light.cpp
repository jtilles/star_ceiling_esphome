#include "starry_night_light.h"
#include "esphome/components/light/light_state.h"

#include <cmath>

namespace esphome {
namespace illuminate {

namespace {

uint8_t to_channel(float value) { return static_cast<uint8_t>(lroundf(value * 255.0f)); }

// Hardware floor is 10%, not 0 - HA's 0.0 brightness is mapped to the floor rather than
// treated as "off" (turning the light off entirely is handled separately in write_state).
uint8_t brightness_to_percent(float brightness) { return static_cast<uint8_t>(lroundf(10.0f + brightness * 90.0f)); }

}  // namespace

light::LightTraits StarryNightLight::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB_WHITE});
  return traits;
}

void StarryNightLight::write_state(light::LightState *state) {
  if (!state->current_values.is_on()) {
    // No documented power on/off command exists for Starry Night; "off" is approximated
    // as commanding black (see README for the reasoning and its limitations).
    this->hub_->send_starry_color(RGBW{0, 0, 0, 0});
    return;
  }

  RGBW color{
      to_channel(state->current_values.get_red()),
      to_channel(state->current_values.get_green()),
      to_channel(state->current_values.get_blue()),
      to_channel(state->current_values.get_white()),
  };
  this->hub_->send_starry_color(color);
  this->hub_->send_star_dimmer(brightness_to_percent(state->current_values.get_brightness()));
}

void StarryAnimationEffect::start() {
  auto *output = static_cast<StarryNightLight *>(this->get_light_state()->get_output());
  output->get_hub()->send_starry_effect(this->effect_);
}

}  // namespace illuminate
}  // namespace esphome
