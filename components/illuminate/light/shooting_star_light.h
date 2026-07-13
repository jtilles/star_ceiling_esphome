// Shooting Star zone platform (SS-firmware units only - see README). Modeled as an
// ESPHome light rather than a button + number/select: a shot is fundamentally "a color,
// fired for a duration," which maps naturally onto a light's color + on/off, and HA
// already gives a color picker for free. The tradeoff is that "duration" doesn't fit HA's
// light data model at all (it's not brightness, not a transition), so it's a config-time
// constant per light entry (`duration:`) rather than something adjustable per-shot from
// the HA UI - add one `illuminate` light entry per zone/duration combination you want
// on a dashboard if you need more than one duration available at runtime.
//
// Turning the light "on" fires "{Zone},{R},{G},{B},{W},{time}*" with the light's current
// color and the configured duration. Turning it "off" sends the manual's explicit
// "{Zone},0,0,0,0,0*" shorthand, canceling any in-flight shot immediately rather than
// waiting for it to time out.
#pragma once

#include "esphome/components/light/light_output.h"
#include "../illuminate.h"

namespace esphome {
namespace illuminate {

class ShootingStarLight : public light::LightOutput {
 public:
  void set_hub(IlluminateHub *hub) { this->hub_ = hub; }
  void set_zone(ShootingStarZone zone) { this->zone_ = zone; }
  void set_duration(ShootingStarDuration duration) { this->duration_ = duration; }

  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;

 protected:
  IlluminateHub *hub_{nullptr};
  ShootingStarZone zone_{ShootingStarZone::ZONE_1};
  ShootingStarDuration duration_{ShootingStarDuration::ONE_MINUTE};
};

}  // namespace illuminate
}  // namespace esphome
