// Starry Night effect renderers (Twinkle/Firefly/Breeze/Random/Rain
// Drops/Jamboree - the six effects in
// components/illuminate/illuminate_protocol.h's StarryEffect enum).
//
// These are new, original implementations aimed at *visual* parity with the
// manual's effect names, not a byte-exact clone of the original firmware's
// animation code - matching this project's explicit non-goal
// (docs/illuminate-enhanced-prd.md: "Not attempting to reverse-engineer or
// reproduce the original firmware's exact internal algorithms
// byte-for-byte... visual parity of effects is the bar, not binary
// equivalence"). The original's actual per-effect timing was never
// extracted from the Ghidra decompile (see docs/illuminate-firmware-notes.md
// - the animation engine section describes the state layout but not the
// per-effect math), so there is nothing to port even if byte-exact parity
// were the goal.
#pragma once

#include "config.h"
#include "types.h"

namespace star_zone {
namespace effects {

// Renders `effect` into `pixels[0..count)`.
//
// `active_color[i]` is the per-pixel color the current color transition
// (see StarZoneController::compute_active_color_) has already resolved for
// pixel i - effects that use "the current color" (Twinkle/Firefly/Breeze/
// Rain Drops/Jamboree) read from it per-pixel so Wipe/Rainbow/Blink keep
// working underneath any effect. RANDOM ignores it entirely by design (its
// whole premise is per-pixel independent random color).
//
// `palette_id` (star_zone::palette::kSolidPaletteId to disable) overrides
// active_color for the same set of effects when a palette has been selected
// via the "!C" enhancement command.
void render(StarryEffect effect, RGBW *pixels, uint8_t count, uint32_t now_ms, uint8_t speed,
            const RGBW *active_color, uint8_t palette_id);

}  // namespace effects
}  // namespace star_zone
