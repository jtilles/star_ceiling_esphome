// IlluminateHub: ESPHome plumbing (Component lifecycle, UARTDevice) for the Fusion
// Synergy "Illuminate 1.0" controller. Deliberately thin — all command-string
// construction lives in illuminate_protocol.h/.cpp, which this class calls into.
//
// STUB: hub class and UART wiring are implemented in a later step (see project plan).
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "illuminate_protocol.h"

namespace esphome {
namespace illuminate {

class IlluminateHub : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void dump_config() override;

  // TODO: send_* wrappers around illuminate::protocol:: functions, called by the
  // light/shooting_star platforms that hold a pointer back to this hub.
};

}  // namespace illuminate
}  // namespace esphome
