# esphome-illuminate

ESPHome external component for the Fusion Synergy "Illuminate 1.0" star ceiling / ambient
lighting controller, controlled natively from Home Assistant over a direct UART connection
instead of the proprietary Bluetooth phone app.

**Status: work in progress.** The protocol layer (`components/illuminate/illuminate_protocol.h/.cpp`)
is implemented and covered by lightweight tests in `tests/`. The ESPHome component itself
(hub, light platforms, shooting star zone) is stubbed out and not yet functional — see
`components/illuminate/` for TODOs.

## TODO before this README is complete

- What this hardware is + link to the official Illuminate 1.0 manual (placeholder).
- SP vs. SS firmware distinction, with a clear warning that Shooting Star commands are
  silently ignored on SP-firmware units.
- Wiring instructions (TX/RX/GND from an ESP8266 to the header the Bluetooth module
  normally plugs into), the 5V logic level note, and the RX-tolerance caveat.
- Daisy-chaining two boards (SP + SS) — open question, see below.

## Open questions (not yet resolved)

- **Daisy-chaining two Illuminate boards** (one SP-firmware for Starry Night/Ambient, one
  SS-firmware for Shooting Star): two UART buses off one ESP8266, two separate ESP8266s
  each with their own `illuminate:` hub, or something else — TBD with repo owner.
- **Starry Night color transitions** (Wipe/Fade/Rainbow/Blink): whether ESPHome's effect
  system is the right fit, versus a separate `select:`, given they're a modifier on top of
  the active color/effect rather than a standalone mode — TBD with repo owner.

## Development

The pure protocol layer has no ESPHome or Arduino dependencies and can be built/tested with
a desktop compiler:

```sh
cd tests
g++ -std=c++17 -Wall -Wextra -I ../components/illuminate \
    test_illuminate_protocol.cpp ../components/illuminate/illuminate_protocol.cpp \
    -o test_illuminate_protocol
./test_illuminate_protocol
```

## License

MIT — see [LICENSE](LICENSE).
