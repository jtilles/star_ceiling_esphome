// Shooting Star zone platform. Modeled as an ESPHome light rather than a button +
// number/select: a shot is fundamentally "a color, fired on a cadence," which maps
// naturally onto a light's color + on/off, and HA already gives a color picker for free.
// The tradeoff is that the firing interval doesn't fit HA's light data model at all (it's
// not brightness, not a transition), so it's a config-time constant per light entry
// (`interval:`) rather than something adjustable per-shot from the HA UI - add one
// `illuminate` light entry per zone/interval combination you want on a dashboard if you
// need more than one interval available at runtime.
//
// Turning the light "on" fires "{Zone},{R},{G},{B},{W},{interval_s}*" with the light's
// current color and the configured interval: one streak immediately, then another every
// interval_s seconds, indefinitely. Turning it "off" sends the explicit
// "{Zone},0,0,0,0,0*" shorthand, canceling the zone immediately.
//
// NOTE: the trailing wire field is seconds BETWEEN shots on star_zone firmware, not the
// original manual's minutes-of-continuous-firing - see illuminate_protocol.h.
#pragma once

#include "esphome/components/light/light_output.h"
#include "../illuminate.h"

namespace esphome {
namespace illuminate {

class ShootingStarLight : public light::LightOutput {
 public:
  void set_hub(IlluminateHub *hub) { this->hub_ = hub; }
  void set_zone(ShootingStarZone zone) { this->zone_ = zone; }
  void set_interval_seconds(uint16_t interval_seconds) { this->interval_seconds_ = interval_seconds; }

  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;

 protected:
  IlluminateHub *hub_{nullptr};
  ShootingStarZone zone_{ShootingStarZone::ZONE_1};
  uint16_t interval_seconds_{30};
};

}  // namespace illuminate
}  // namespace esphome
