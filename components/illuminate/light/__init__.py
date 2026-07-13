import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.components.light.effects import register_effect, validate_effects
from esphome.components.light.types import LightEffect
from esphome.const import CONF_EFFECTS, CONF_NAME, CONF_OUTPUT_ID, CONF_TYPE, CONF_DEFAULT_TRANSITION_LENGTH

from .. import (
    illuminate_ns,
    IlluminateHub,
    StarryEffect,
    AmbientEffect,
    ShootingStarZone,
    ShootingStarDuration,
)
from ..const import CONF_ILLUMINATE_ID

CODEOWNERS = ["@jtilles"]
DEPENDENCIES = ["illuminate"]

StarryNightLight = illuminate_ns.class_("StarryNightLight", light.LightOutput)
AmbientLight = illuminate_ns.class_("AmbientLight", light.LightOutput)
ShootingStarLight = illuminate_ns.class_("ShootingStarLight", light.LightOutput)

AmbientEffectLightEffect = illuminate_ns.class_("AmbientEffectLightEffect", LightEffect)
StarryAnimationEffect = illuminate_ns.class_("StarryAnimationEffect", LightEffect)

CONF_ZONE = "zone"
CONF_DURATION = "duration"
CONF_SPEED = "speed"

TYPE_STARRY_NIGHT = "starry_night"
TYPE_AMBIENT = "ambient"
TYPE_SHOOTING_STAR = "shooting_star"

ZONE_OPTIONS = {
    "zone_1": ShootingStarZone.ZONE_1,
    "zone_2": ShootingStarZone.ZONE_2,
    "zone_3": ShootingStarZone.ZONE_3,
}

# Shooting Star's "0"/off duration is handled by the light's own on/off, not a selectable
# shot duration - only the four real shot lengths from the manual are offered here.
DURATION_OPTIONS = {
    "1min": ShootingStarDuration.ONE_MINUTE,
    "2min": ShootingStarDuration.TWO_MINUTES,
    "5min": ShootingStarDuration.FIVE_MINUTES,
    "10min": ShootingStarDuration.TEN_MINUTES,
}

# The six Starry Night animations (Twinkle..Jamboree) map naturally onto ESPHome's normal
# light effects: they're mutually-exclusive named modes, exactly like WLED/core effects,
# and each is a single fire-and-forget command - the hardware runs the animation itself
# afterwards, so these effects only need to act once on start(), never on apply().
STARRY_EFFECTS = [
    ("illuminate_twinkle", "Twinkle", StarryEffect.TWINKLE),
    ("illuminate_firefly", "Firefly", StarryEffect.FIREFLY),
    ("illuminate_breeze", "Breeze", StarryEffect.BREEZE),
    ("illuminate_random", "Random", StarryEffect.RANDOM),
    ("illuminate_rain_drops", "Rain Drops", StarryEffect.RAIN_DROPS),
    ("illuminate_jamboree", "Jamboree", StarryEffect.JAMBOREE),
]
STARRY_EFFECT_NAMES = [key for key, _, _ in STARRY_EFFECTS]

# Fade/Blink are likewise modeled as ordinary light effects. Each carries its own "speed"
# (150-255) as a fixed config-time parameter on the effect entry, the same way core
# effects like `pulse` carry a fixed transition_length/update_interval - HA's effect
# picker only lets a user pick *which* effect is active, not tune a running effect's
# parameters, so a runtime speed control wouldn't be reachable from the HA UI anyway.
AMBIENT_EFFECTS = [
    ("illuminate_ambient_fade", "Fade", AmbientEffect.FADE),
    ("illuminate_ambient_blink", "Blink", AmbientEffect.BLINK),
]
AMBIENT_EFFECT_NAMES = [key for key, _, _ in AMBIENT_EFFECTS]


def _register_starry_effects():
    for key, default_name, enum_value in STARRY_EFFECTS:

        def _to_code_factory(enum_value=enum_value):
            async def _to_code(config, effect_id):
                return cg.new_Pvariable(effect_id, config[CONF_NAME], enum_value)

            return _to_code

        register_effect(key, StarryAnimationEffect, default_name, {})(_to_code_factory())


def _register_ambient_effects():
    for key, default_name, enum_value in AMBIENT_EFFECTS:

        def _to_code_factory(enum_value=enum_value):
            async def _to_code(config, effect_id):
                return cg.new_Pvariable(effect_id, config[CONF_NAME], enum_value, config[CONF_SPEED])

            return _to_code

        schema = {cv.Optional(CONF_SPEED, default=200): cv.int_range(min=150, max=255)}
        register_effect(key, AmbientEffectLightEffect, default_name, schema)(_to_code_factory())


_register_starry_effects()
_register_ambient_effects()

# The Illuminate protocol has no ESPHome-side notion of a smooth transition (Starry
# Night/Ambient color changes take effect immediately on the hardware, or - for Starry
# Night's Fade - are driven by the hardware's own Fade color transition, not ESPHome's).
# Left at ESPHome's default, `default_transition_length` would make the light ramp
# through many intermediate RGBW values over ~1s, each triggering its own UART write at
# 9600 baud - needless bus traffic for a light with no real transition. Default it to 0s;
# users who want a specific fixed color to fade in over time should reach for the
# hardware's own Fade color transition (illuminate_color_transition select) instead.
_BASE_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.Required(CONF_ILLUMINATE_ID): cv.use_id(IlluminateHub),
        cv.Optional(CONF_DEFAULT_TRANSITION_LENGTH, default="0s"): cv.positive_time_period_milliseconds,
    }
)

STARRY_NIGHT_SCHEMA = _BASE_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(StarryNightLight),
        cv.Optional(CONF_EFFECTS): validate_effects(STARRY_EFFECT_NAMES),
    }
)

AMBIENT_SCHEMA = _BASE_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(AmbientLight),
        cv.Optional(CONF_EFFECTS): validate_effects(AMBIENT_EFFECT_NAMES),
    }
)

# Only functional on SS-firmware units - see README. Omit this `type: shooting_star`
# light entirely from your config if your board is SP-firmware; there is nothing else to
# disable, it's simply never instantiated unless you add one.
SHOOTING_STAR_SCHEMA = _BASE_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(ShootingStarLight),
        cv.Required(CONF_ZONE): cv.enum(ZONE_OPTIONS, lower=True),
        cv.Optional(CONF_DURATION, default="1min"): cv.enum(DURATION_OPTIONS, lower=True),
    }
)

CONFIG_SCHEMA = cv.typed_schema(
    {
        TYPE_STARRY_NIGHT: STARRY_NIGHT_SCHEMA,
        TYPE_AMBIENT: AMBIENT_SCHEMA,
        TYPE_SHOOTING_STAR: SHOOTING_STAR_SCHEMA,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)

    hub = await cg.get_variable(config[CONF_ILLUMINATE_ID])
    cg.add(var.set_hub(hub))

    if config[CONF_TYPE] == TYPE_SHOOTING_STAR:
        cg.add(var.set_zone(config[CONF_ZONE]))
        cg.add(var.set_duration(config[CONF_DURATION]))
