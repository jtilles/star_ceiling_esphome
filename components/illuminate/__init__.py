import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@jtilles"]
DEPENDENCIES = ["uart"]

# Two illuminate: hubs can coexist on one ESP8266 (e.g. one for an SP-firmware board's
# Starry Night/Ambient zones, one for a second, SS-firmware board's Shooting Star zones),
# each on its own dedicated uart: bus.
MULTI_CONF = True

illuminate_ns = cg.esphome_ns.namespace("illuminate")
IlluminateHub = illuminate_ns.class_("IlluminateHub", cg.Component, uart.UARTDevice)

# Shared with the light/ and select/ platforms below, which reference these to build
# protocol command arguments (e.g. StarryEffect.TWINKLE) without redeclaring them.
StarryEffect = illuminate_ns.enum("StarryEffect", is_class=True)
ColorTransition = illuminate_ns.enum("ColorTransition", is_class=True)
AmbientEffect = illuminate_ns.enum("AmbientEffect", is_class=True)
ShootingStarZone = illuminate_ns.enum("ShootingStarZone", is_class=True)
ShootingStarDuration = illuminate_ns.enum("ShootingStarDuration", is_class=True)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(IlluminateHub),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

# The Illuminate "Main" MCU expects a raw, unbuffered 9600 8N1 stream on the header the
# HM-10 BLE module normally occupies. require_tx/require_rx also stops another uart
# device accidentally claiming the same TX/RX pins on this bus.
FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "illuminate",
    baud_rate=9600,
    require_tx=True,
    require_rx=True,
    data_bits=8,
    parity="NONE",
    stop_bits=1,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
