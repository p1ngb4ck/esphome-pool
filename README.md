# esphome-pool

## Purpose
Smaller additions that belong to no other topic: AC current and voltage sensors (ACS712, ZMPT101B) with RMS measured
from ADS1115 burst samples, ESP32-S31 support of the ADC, an S/PDIF speaker on a single GPIO, select option changes
with a revision, and small LVGL, web server, API and IR / RF fixes.

## How it works
- `acs712` / `zmpt101b` read an `ads1115` channel: `ads1115` takes the samples of one measurement as a burst on one
  channel (no channel switch in between), the sensors compute RMS current / voltage and optionally power.
- `adc` gains ESP32-S31 support (variant guards, line-fitting calibration).
- `spdif` encodes IEC 60958 in its own task and sends it out of one GPIO through the PARLIO peripheral.
- `select`, `ir_rf_base` and the `lvgl` / `web_server` / `api` branches are small fixes to existing components.

## Quick start
Chips: ESP32, ESP32-S2, ESP32-S3, ESP32-P4
Needs: [esphome-mipi-display](https://github.com/p1ngb4ck/esphome-mipi-display) (`i2c`)

```yaml
external_components:
  - source: github://p1ngb4ck/esphome-pool@main
    components: [ads1115, acs712]
  - source: github://p1ngb4ck/esphome-mipi-display@main
    components: [i2c]
i2c: {sda: GPIO4, scl: GPIO5}
ads1115: [{address: 0x48}]
sensor:
  - {platform: ads1115, id: acs_raw, multiplexer: A0_GND, gain: 6.144}
  - platform: acs712
    sensor: acs_raw
    name: Current
    model: 20A
    line_voltage: 230
```

Use your board's pins. S/PDIF out (ESP32-P4): `speaker:` `platform: spdif`, `pin:`; it takes `audio` from esphome-audio.

## Main options

| Option | What it does | Default |
|---|---|---|
| [`acs712: sensor`](CONFIGURATION.md#acs712-sensor) | the voltage sampler (an `ads1115` sensor) | required |
| [`acs712: model`](CONFIGURATION.md#acs712-sensor) | `5A`, `20A`, `30A` (sensitivity) | `20A` |
| [`line_voltage` / `line_voltage_sensor`](CONFIGURATION.md#acs712-sensor) | fixed or measured voltage for `power` | - |
| [`zmpt101b: sensitivity`](CONFIGURATION.md#zmpt101b-sensor) | calibration factor, 0.1-100 | - |
| [`zmpt101b: frequency`](CONFIGURATION.md#zmpt101b-sensor) | mains frequency, 50 or 60 | `50` |
| [`spdif: pin`](CONFIGURATION.md#spdif-speaker) | the output GPIO | required |
| [`spdif: sample_rate`](CONFIGURATION.md#spdif-speaker) | `32000`, `44100` or `48000` | `48000` |
| [other changes](CONFIGURATION.md#other-changes) | adc, ads1115 burst, select revision, lvgl | - |

## Catalog
- Components (folders on main; options):
  [acs712](https://github.com/p1ngb4ck/esphome-pool/tree/main/esphome/components/acs712)
  ([options](CONFIGURATION.md#acs712-sensor)),
  [zmpt101b](https://github.com/p1ngb4ck/esphome-pool/tree/main/esphome/components/zmpt101b)
  ([options](CONFIGURATION.md#zmpt101b-sensor)),
  [ads1115](https://github.com/p1ngb4ck/esphome-pool/tree/main/esphome/components/ads1115),
  [adc](https://github.com/p1ngb4ck/esphome-pool/tree/main/esphome/components/adc),
  [select](https://github.com/p1ngb4ck/esphome-pool/tree/main/esphome/components/select),
  [ir_rf_base](https://github.com/p1ngb4ck/esphome-pool/tree/main/esphome/components/ir_rf_base)
  ([changes](CONFIGURATION.md#other-changes)),
  [spdif](https://github.com/p1ngb4ck/esphome-pool/tree/main/esphome/components/spdif)
  ([options](CONFIGURATION.md#spdif-speaker)).
- Changed here, owned elsewhere: `lvgl` ([esphome-mipi-display](https://github.com/p1ngb4ck/esphome-mipi-display)),
  `web_server` ([esphome-storage-interface](https://github.com/p1ngb4ck/esphome-storage-interface)), `api`
  ([esphome-network](https://github.com/p1ngb4ck/esphome-network)).
- Branches: [main](https://github.com/p1ngb4ck/esphome-pool/tree/main),
  [pr/acs712](https://github.com/p1ngb4ck/esphome-pool/tree/pr/acs712),
  [pr/adc](https://github.com/p1ngb4ck/esphome-pool/tree/pr/adc),
  [pr/ads1115](https://github.com/p1ngb4ck/esphome-pool/tree/pr/ads1115),
  [pr/api-crash-report-once](https://github.com/p1ngb4ck/esphome-pool/tree/pr/api-crash-report-once),
  [pr/ir-rf-base-define](https://github.com/p1ngb4ck/esphome-pool/tree/pr/ir-rf-base-define),
  [pr/lvgl-color-picker-setters](https://github.com/p1ngb4ck/esphome-pool/tree/pr/lvgl-color-picker-setters),
  [pr/lvgl-keyboard-text](https://github.com/p1ngb4ck/esphome-pool/tree/pr/lvgl-keyboard-text),
  [pr/lvgl-menu-back-button](https://github.com/p1ngb4ck/esphome-pool/tree/pr/lvgl-menu-back-button),
  [pr/select](https://github.com/p1ngb4ck/esphome-pool/tree/pr/select),
  [pr/spdif](https://github.com/p1ngb4ck/esphome-pool/tree/pr/spdif),
  [pr/web-server-entity-row-css](https://github.com/p1ngb4ck/esphome-pool/tree/pr/web-server-entity-row-css),
  [pr/zmpt101b](https://github.com/p1ngb4ck/esphome-pool/tree/pr/zmpt101b).
- Needed repos: [esphome-mipi-display](https://github.com/p1ngb4ck/esphome-mipi-display) (`i2c`); for `spdif`
  [esphome-audio](https://github.com/p1ngb4ck/esphome-audio) (`audio`).
- All topics together: `github://p1ngb4ck/esphome@dev`.

---

# Detailed version

Smaller additions that belong to no other topic: AC current and voltage sensors (ACS712, ZMPT101B) with RMS measured
from ADS1115 burst samples, burst sampling for the ADS1115 and ESP32-S31 support of the ESP32 ADC, an S/PDIF speaker
on a single GPIO, select option changes sent to the web server as a change, and two LVGL fixes.

## Examples

Mains current and power with an ACS712 on an ADS1115:

```yaml
external_components:
  - source: github://p1ngb4ck/esphome-pool@main
    components: [ads1115, acs712]
  - source: github://p1ngb4ck/esphome-mipi-display@main
    components: [i2c]

i2c:
  sda: GPIO21
  scl: GPIO22

ads1115:
  - address: 0x48

sensor:
  - platform: ads1115
    id: acs_raw
    multiplexer: A0_GND
    gain: 6.144
  - platform: acs712
    sensor: acs_raw
    name: Current
    model: 20A
    line_voltage: 230
    power:
      name: Power
```

Use your board's pins (GPIO21 / GPIO22 are ESP32 pins). Take only these components from here: `main` also carries
this repo's state of `lvgl` and `web_server`, which would replace those of their owners
([esphome-mipi-display](https://github.com/p1ngb4ck/esphome-mipi-display),
[esphome-storage-interface](https://github.com/p1ngb4ck/esphome-storage-interface)).

Digital audio out to an amplifier or receiver over S/PDIF:

```yaml
external_components:
  - source: github://p1ngb4ck/esphome-pool@main
    components: [spdif]
  - source: github://p1ngb4ck/esphome-audio@main
    components: [audio]

speaker:
  - platform: spdif
    pin: GPIO15
    sample_rate: 48000
```

## More

**[CONFIGURATION.md](CONFIGURATION.md)**: every option of `acs712`, `zmpt101b` and the `spdif` speaker, and what the
other changes do.

## Branches

One branch per change; `main` contains every branch. Base: ESPHome `dev` at `a5a75738ad`.

- `pr/adc` (ESP32-S31): adc: ESP32-S31 support (variant guards, line-fitting calibration).
- `pr/acs712` (all): `acs712`: ACS712 current sensor (RMS) from ADS1115 bursts, power with a line voltage.
- `pr/zmpt101b` (all): `zmpt101b`: ZMPT101B AC voltage sensor (RMS) from ADS1115 bursts.
- `pr/ads1115` (all): ads1115: burst measurements on one channel (for RMS sensors) without a channel switch in
  between.
- `pr/select` (all): select: option list changes carry a revision; web_server sends only the change.
- `pr/spdif` (see below): `spdif`: IEC 60958 (S/PDIF) speaker on one GPIO (the PARLIO peripheral).
- `pr/lvgl-color-picker-setters` (all): lvgl color picker: LVGL 9.6's flag setters.
- `pr/lvgl-keyboard-text` (all): lvgl: a keyboard trigger's text is its textarea's (empty without one).
- `pr/lvgl-menu-back-button` (all): lvgl: the menu's root back-button check reads the back buttons and depth itself
  (no deprecated `lv_menu_back_button_is_root`).
- `pr/ir-rf-base-define` (all): ir_rf_base: `USE_IR_RF` whenever the component is loaded.
- `pr/web-server-entity-row-css` (all): web_server: the version 3 entity table's value cell may shrink
  (`min-width: 0`).
- `pr/api-crash-report-once` (all): api: the stored crash report goes only to the connection that subscribes to logs.

`pr/spdif` chips: ESP32 chips with PARLIO (C5, C6, H2, P4). The component has no variant check yet: a build for a
chip without PARLIO fails at compile time, not at validation.

`pr/lvgl-menu-back-button` and `pr/web-server-entity-row-css` (and `main`) currently carry more than their one-line
change: this repo's whole `lvgl` runtime and the file-API `web_server`; they are to be reset to upstream plus that
change.

`pr/acs712` and `pr/zmpt101b` need `pr/ads1115` (the burst measurement); they are still based on `pr/adc` and are to
be rebased on `pr/ads1115`. All topics together: `github://p1ngb4ck/esphome@dev`.
`external_components` replaces components only; the ESPHome core of the build host stays as installed.

## License

- ESPHome's own files, also where this repository changes them: the [ESPHome License](LICENSE) (MIT for the Python
  code and everything else, GPLv3 for the C++ runtime files).
- New files written for this repository (new components, their code, tests and documentation): MIT,
  [LICENSE-MIT](LICENSE-MIT).

## Thanks

Everyone this project builds on: [ATTRIBUTIONS.md](https://github.com/p1ngb4ck/esphome/blob/dev/ATTRIBUTIONS.md).

## Branch structure

Each `pr/` branch holds one part of this topic on top of upstream and says in its `BRANCH.md` what it is based on.
`main` holds all of them.

| Branch | Based on |
|---|---|
| `pr/acs712` | `pr/adc` (needs `pr/ads1115`; rebase pending) |
| `pr/adc` | upstream only |
| `pr/ads1115` | upstream only |
| `pr/api-crash-report-once` | upstream only |
| `pr/ir-rf-base-define` | upstream only |
| `pr/lvgl-color-picker-setters` | upstream only |
| `pr/lvgl-keyboard-text` | upstream only |
| `pr/lvgl-menu-back-button` | upstream only |
| `pr/select` | upstream only |
| `pr/spdif` | upstream only |
| `pr/web-server-entity-row-css` | upstream only |
| `pr/zmpt101b` | `pr/adc` (needs `pr/ads1115`; rebase pending) |
