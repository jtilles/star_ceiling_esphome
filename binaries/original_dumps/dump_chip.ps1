<#
.SYNOPSIS
    Dumps flash (hex+bin), EEPROM (hex), and fuse/lock bits from an ATmega328P
    via avrdude/USBasp into a chip-specific subdirectory.

.USAGE
    .\dump_chip.ps1 -ChipName TOP-STAR
    .\dump_chip.ps1 -ChipName BOT-STAR
    .\dump_chip.ps1 -ChipName RGBW

    Or just run it with no args and it will prompt you for the chip name.

.OUTPUT
    Creates .\<ChipName>\ containing:
        main.hex          - flash dump, Intel HEX
        main.bin          - flash dump, raw binary
        main_eeprom.hex   - EEPROM dump, Intel HEX
        main_fuses.txt    - lfuse/hfuse/efuse/lock readout
#>

param(
    [Parameter(Mandatory = $false)]
    [string]$ChipName,

    [string]$BaseDir = "C:\Users\josh\OneDrive\Documents\Projects\Fiber_optic_star_ceiling_esphome\binaries\original_dumps",

    [string]$Programmer = "usbasp",

    [string]$Part = "m328p"
)

# --- Prompt for chip name if not supplied ---
if ([string]::IsNullOrWhiteSpace($ChipName)) {
    $ChipName = Read-Host "Enter chip name (e.g. TOP-STAR, BOT-STAR, RGBW)"
}
if ([string]::IsNullOrWhiteSpace($ChipName)) {
    Write-Error "No chip name provided. Aborting."
    exit 1
}

$OutDir = Join-Path $BaseDir $ChipName

if (-not (Test-Path $OutDir)) {
    New-Item -ItemType Directory -Path $OutDir -Force | Out-Null
    Write-Host "Created directory: $OutDir" -ForegroundColor Cyan
}
else {
    Write-Host "Directory already exists: $OutDir" -ForegroundColor Yellow
    $resp = Read-Host "Files may be overwritten. Continue? (y/n)"
    if ($resp -ne "y") {
        Write-Host "Aborted."
        exit 0
    }
}

$baseName = [System.IO.Path]::GetFileName($ChipName)
$hexPath = Join-Path $OutDir "$baseName.hex"
$binPath = Join-Path $OutDir "$baseName.bin"
$eepromPath = Join-Path $OutDir "$baseName.eeprom.hex"
$fusesPath = Join-Path $OutDir "$baseName.fuses.txt"

function Run-Avrdude {
    param([string[]]$ArgumentList, [string]$Description)

    Write-Host "`n--- $Description ---" -ForegroundColor Green
    Write-Host "avrdude $($ArgumentList -join ' ')" -ForegroundColor DarkGray

    & avrdude @ArgumentList
    if ($LASTEXITCODE -ne 0) {
        Write-Warning "$Description exited with code $LASTEXITCODE"
        return $false
    }
    return $true
}

# --- Confirm chip is connected before starting ---
Write-Host "`nConnect the $ChipName board now, then press Enter to begin." -ForegroundColor Cyan
Read-Host | Out-Null

Push-Location $OutDir
try {
    # --- 1. Flash dump: HEX ---
    $ok1 = Run-Avrdude -Description "Reading flash (HEX)" -ArgumentList @(
        "-c", $Programmer,
        "-p", $Part,
        "-U", "flash:r:$baseName.hex:i"
    )

    # --- 2. Flash dump: BIN ---
    $ok2 = Run-Avrdude -Description "Reading flash (BIN)" -ArgumentList @(
        "-c", $Programmer,
        "-p", $Part,
        "-U", "flash:r:$baseName.bin:r"
    )

    # --- 3. EEPROM dump ---
    $ok3 = Run-Avrdude -Description "Reading EEPROM (HEX)" -ArgumentList @(
        "-c", $Programmer,
        "-p", $Part,
        "-U", "eeprom:r:$baseName.eeprom.hex:i"
    )

    # --- 4. Fuses/lock bits -> capture console output to a text file ---
    Write-Host "`n--- Reading fuses/lock bits ---" -ForegroundColor Green
    $fuseArgs = @(
        "-c", $Programmer,
        "-p", $Part,
        "-U", "lfuse:r:-:h",
        "-U", "hfuse:r:-:h",
        "-U", "efuse:r:-:h",
        "-U", "lock:r:-:h"
    )
    Write-Host "avrdude $($fuseArgs -join ' ')" -ForegroundColor DarkGray

    $fuseOutput = & avrdude @fuseArgs 2>&1
    $fuseOutput | Out-String | Set-Content -Path $fusesPath
    $fuseOutput | ForEach-Object { Write-Host $_ }

    $ok4 = $LASTEXITCODE -eq 0
}
finally {
    Pop-Location
}

# --- Summary ---
Write-Host "`n===== Summary for $ChipName =====" -ForegroundColor Cyan
Write-Host ("Flash HEX  : {0}  [{1}]" -f $hexPath, $(if ($ok1) { "OK" } else { "FAILED" }))
Write-Host ("Flash BIN  : {0}  [{1}]" -f $binPath, $(if ($ok2) { "OK" } else { "FAILED" }))
Write-Host ("EEPROM HEX : {0}  [{1}]" -f $eepromPath, $(if ($ok3) { "OK" } else { "FAILED" }))
Write-Host ("Fuses TXT  : {0}  [{1}]" -f $fusesPath, $(if ($ok4) { "OK" } else { "FAILED" }))

if ($ok1 -and $ok2 -and $ok3 -and $ok4) {
    Write-Host "`nAll reads completed successfully for $ChipName." -ForegroundColor Green
}
else {
    Write-Warning "`nOne or more reads failed for $ChipName - check connections and re-run if needed."
}