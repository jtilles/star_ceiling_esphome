#include "illuminate_color_transition_select.h"

namespace esphome {
namespace illuminate {

void IlluminateColorTransitionSelect::control(const std::string &value) {
  ColorTransition transition;
  if (value == "Wipe") {
    transition = ColorTransition::WIPE;
  } else if (value == "Fade") {
    transition = ColorTransition::FADE;
  } else if (value == "Rainbow") {
    transition = ColorTransition::RAINBOW;
  } else {
    transition = ColorTransition::BLINK;
  }
  this->hub_->send_color_transition(transition);
  this->publish_state(value);
}

}  // namespace illuminate
}  // namespace esphome
