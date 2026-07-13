// Companion select for Starry Night's four color transitions (Wipe/Fade/Rainbow/Blink).
// See __init__.py for why these are a select rather than folded into the Starry Night
// light's effects: list.
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/select/select.h"
#include "../illuminate.h"

namespace esphome {
namespace illuminate {

class IlluminateColorTransitionSelect : public select::Select, public Component {
 public:
  void set_hub(IlluminateHub *hub) { this->hub_ = hub; }

 protected:
  void control(const std::string &value) override;

  IlluminateHub *hub_{nullptr};
};

}  // namespace illuminate
}  // namespace esphome
