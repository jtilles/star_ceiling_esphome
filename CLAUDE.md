# Fiber Optic Star Ceiling — Illuminate ESPHome Project

Reverse-engineered control for a Fusion Synergy "Illuminate 1.0" star ceiling
controller. Two layers of reverse engineering here, at different levels:

1. **RS232 command protocol** (Main MCU, BLE/UART) — fully solved and
   documented in [README.md](README.md): effects, colors, dimmers, shooting
   star, ambient light. Implemented as a proper ESPHome external component
   in `components/illuminate/`, published at
   https://github.com/jtilles/star_ceiling_esphome.
2. **Physical LED protocol** (zone MCUs → LED strings) — reverse-engineered
   via `docs/tooling/` (see `docs/tooling/CLAUDE.md`) to the point of
   shipping a full custom replacement firmware, `firmware/star_zone/` (see
   `firmware/star_zone/CLAUDE.md`), that has been bench-flashed and
   iterated against real Top-Star/Bot-Star hardware. RGBW wire byte order
   is still an open/unconfirmed guess in that firmware even so — see its
   own "Open questions" section.

See also `binaries/original_dumps/CLAUDE.md` for the unrelated one-off
`dump_chip.ps1` hardware utility.
