import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_ID

from .. import illuminate_ns, IlluminateHub
from ..const import CONF_ILLUMINATE_ID

CODEOWNERS = ["@jtilles"]
DEPENDENCIES = ["illuminate"]

# Wipe/Fade/Rainbow/Blink are a modifier layered on top of whatever Starry Night
# color/effect is currently active (the manual lists them separately from the six
# animation effects, and they take no parameters of their own) rather than a mode in the
# same sense Twinkle/Firefly/etc. are. Folding all ten into one ESPHome `effects:` list
# would force a user to pick only one of "Twinkle" or "Wipe" at a time in HA, when the
# hardware actually lets both be active together. A companion select is the more honest
# fit for that relationship and keeps both independently visible/settable in HA.
IlluminateColorTransitionSelect = illuminate_ns.class_(
    "IlluminateColorTransitionSelect", select.Select, cg.Component
)

TRANSITION_OPTIONS = ["Wipe", "Fade", "Rainbow", "Blink"]

CONFIG_SCHEMA = select.select_schema(IlluminateColorTransitionSelect).extend(
    {
        cv.Required(CONF_ILLUMINATE_ID): cv.use_id(IlluminateHub),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await select.register_select(var, config, options=TRANSITION_OPTIONS)

    hub = await cg.get_variable(config[CONF_ILLUMINATE_ID])
    cg.add(var.set_hub(hub))
