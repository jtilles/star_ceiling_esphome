#include "shooting_star_light.h"
#include "esphome/components/light/light_state.h"

#include <cmath>

namespace esphome {
namespace illuminate {

light::LightTraits ShootingStarLight::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB_WHITE});
  return traits;
}

void ShootingStarLight::write_state(light::LightState *state) {
  if (!state->current_values.is_on()) {
    this->hub_->send_shooting_star_off(this->zone_);
    return;
  }

  // Unlike the other two lights, the hardware has no independent brightness channel for
  // shooting stars, so brightness is folded into the RGBW values here (via the
  // brightness-baked-in helper) rather than ignored, so the HA brightness slider still
  // does something sensible.
  float red, green, blue, white;
  state->current_values_as_rgbw(&red, &green, &blue, &white);
  RGBW color{
      static_cast<uint8_t>(lroundf(red * 255.0f)),
      static_cast<uint8_t>(lroundf(green * 255.0f)),
      static_cast<uint8_t>(lroundf(blue * 255.0f)),
      static_cast<uint8_t>(lroundf(white * 255.0f)),
  };
  this->hub_->send_shooting_star(this->zone_, color, this->duration_);
}

}  // namespace illuminate
}  // namespace esphome
