// STUB: Ambient Light platform (on/off, RGBW color, effects, brightness -> strip
// dimmer). Implemented first among the two light platforms (simplest command shape) —
// see project plan.
#pragma once

#include "esphome/components/light/light_output.h"
#include "../illuminate.h"

namespace esphome {
namespace illuminate {

class AmbientLight : public light::LightOutput {
 public:
  // TODO: implement.
};

}  // namespace illuminate
}  // namespace esphome
