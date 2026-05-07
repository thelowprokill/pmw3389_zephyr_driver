# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Zephyr RTOS **out-of-tree driver module** for the PixArt PMW3389 optical motion sensor. It is consumed by other Zephyr projects via the west manifest or `EXTRA_ZEPHYR_MODULES`.

## Build Commands

This module is not built standalone — it is integrated into a Zephyr application. The `samples/sensor/pmw3389/` directory contains a working example targeting the Teensy 4.1 board.

From within the sample directory (or any app using this module):

```sh
# Standard build
west build -b teensy41

# Clean build
west build --pristine -b teensy41

# Flash
west flash

# View output (USB CDC-ACM)
cat /dev/ttyACM0
```

**Prerequisites**: Zephyr v4.3.0+, west, CMake, a compatible LPSPI driver module (see `samples/sensor/pmw3389/README.md`).

## Architecture

### Module Layout

- `drivers/sensor/pixart/pmw3389/pmw3389.c` — Core driver (~504 lines)
- `include/zephyr/drivers/sensor/pmw3389.h` — Public API (custom sensor channels)
- `dts/bindings/sensor/pixart,pmw3389.yaml` — Device tree binding
- `zephyr/module.yml` — Zephyr module declaration (CMake + Kconfig + DTS roots)
- `samples/sensor/pmw3389/` — Teensy 4.1 example application

### Driver Internals

The driver uses **SPI with manual chip-select** and strict timing delays (`k_busy_wait`) required by the PMW3389 datasheet:

| Timing | Value |
|--------|-------|
| T_SWW (write-to-write) | 180 µs |
| T_SWR (read after write) | 180 µs |
| T_SRW (write after read) | 20 µs |
| T_SRAD (address-to-data) | 160 µs |
| T_SRAD_MOTBR (burst read) | 35 µs |

**Initialization** (`pmw3389_init`) follows the 11-step datasheet sequence: power reset → 50 ms wait → dummy register reads → configuration writes → poll register 0x3D for 0xC0 → REST/CPI config → product ID verification (0x47 / inverse 0xB8).

**Motion fetch** has two modes controlled by `#define FETCH_USING_BURST_READ` at the top of `pmw3389.c`:
- Burst read (default): reads Motion, Observation, delta_x, delta_y, quality, and raw stats in one transaction
- Register-by-register fallback

**Data output**: `delta_x` and `delta_y` are 16-bit signed counts. `channel_get` converts them to inches using `counts / (39.3700787 × CPI)`. The `SENSOR_CHAN_PMW3389_DISTANCE_X/Y` channels are defined in `pmw3389.h`.

### Device Tree Binding

The binding (`pixart,pmw3389`) inherits from `spi-device.yaml` and requires one property:

- `resolution`: integer CPI value, 50–16000, must be a multiple of 50.

**Important**: At low resolutions the ±32767 accumulator can overflow quickly (e.g., 200 CPI overflows in ~0.5 s at 8 m/s). Applications must fetch frequently or use higher CPI.

Maximum SPI clock: **2 MHz** (datasheet limit).

### Kconfig

`CONFIG_PMW3389` (default `y`) gates the driver. It depends on `DT_HAS_PIXART_PMW3389_ENABLED` (i.e., the sensor must appear in the device tree) and auto-selects `SPI` and `SENSOR`.

## Known Limitations

- `pwm3389_get_raw_data()` (1296-byte pixel dump) is marked **untested / may be broken**.
- SROM download is a TODO comment in `pmw3389_init` — may be required for some firmware configurations.
