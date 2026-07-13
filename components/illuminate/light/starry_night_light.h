// Starry Night platform: RGBW color -> "c,R,G,B,W*", brightness -> "D,{pct}*", and the
// six animation effects (Twinkle/Firefly/Breeze/Random/Rain Drops/Jamboree) exposed as
// ordinary ESPHome light effects, each a one-shot command fired on activation (the
// hardware runs the animation itself afterwards).
//
// Unlike Ambient Light, the manual has no "no effect" command for Starry Night, so
// deselecting an effect in HA intentionally sends nothing - the hardware just keeps
// running whatever animation was last selected.
#pragma once

#include "esphome/components/light/light_output.h"
#include "esphome/components/light/light_effect.h"
#include "../illuminate.h"

namespace esphome {
namespace illuminate {

class StarryNightLight : public light::LightOutput {
 public:
  void set_hub(IlluminateHub *hub) { this->hub_ = hub; }
  IlluminateHub *get_hub() const { return this->hub_; }

  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;

 protected:
  IlluminateHub *hub_{nullptr};
};

// Backs each of the six "illuminate_starry_*" effect entries.
class StarryAnimationEffect : public light::LightEffect {
 public:
  StarryAnimationEffect(const char *name, StarryEffect effect) : LightEffect(name), effect_(effect) {}

  void start() override;
  void apply() override {}

 protected:
  StarryEffect effect_;
};

}  // namespace illuminate
}  // namespace esphome
