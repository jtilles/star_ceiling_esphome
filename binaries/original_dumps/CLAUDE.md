## dump_chip.ps1

Dumps flash/EEPROM/fuses from ATmega328P boards (e.g. TOP-STAR) via
`avrdude`. Needs `avrdude` in PATH. Writes chip-named files
(`<ChipName>.hex`, `.bin`, `.eeprom.hex`, `.fuses.txt`) into a per-chip
subfolder; run with `-ChipName <name>`. Unrelated to the Illuminate/Saleae
work elsewhere in this repo — a separate one-off hardware utility for a
different task (reading the stock firmware off a board).
