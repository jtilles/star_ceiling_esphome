// Ambient Light platform: RGBW color -> "A,R,G,B,W*", brightness -> "d,{pct}*", and two
// hardware effects (Fade/Blink) exposed as ordinary ESPHome light effects. Each effect's
// "speed" (150-255) is a config-time parameter on the effect entry itself (like core
// effects such as `pulse`'s transition_length) rather than a runtime HA control, since HA's
// effect picker only lets a user choose which effect is active, not tune its parameters
// live.
#pragma once

#include "esphome/components/light/light_output.h"
#include "esphome/components/light/light_effect.h"
#include "../illuminate.h"

namespace esphome {
namespace illuminate {

class AmbientLight : public light::LightOutput {
 public:
  void set_hub(IlluminateHub *hub) { this->hub_ = hub; }
  IlluminateHub *get_hub() const { return this->hub_; }

  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;

 protected:
  IlluminateHub *hub_{nullptr};
};

// Backs both the "illuminate_ambient_fade" and "illuminate_ambient_blink" effect entries.
// Fires the one-shot "e,{speed}" / "k,{speed}" select command on activation; the hardware
// runs the animation itself from then on, so apply() has nothing to do every loop.
class AmbientEffectLightEffect : public light::LightEffect {
 public:
  AmbientEffectLightEffect(const char *name, AmbientEffect effect, uint8_t speed)
      : LightEffect(name), effect_(effect), speed_(speed) {}

  void start() override;
  void apply() override {}

 protected:
  AmbientEffect effect_;
  uint8_t speed_;
};

}  // namespace illuminate
}  // namespace esphome
