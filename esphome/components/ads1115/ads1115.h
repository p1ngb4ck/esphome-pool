#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

#ifdef USE_ESP32
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#endif

namespace esphome::ads1115 {

class ADS1115Component final : public Component, public i2c::I2CDevice {
 public:
  explicit ADS1115Component(bool continuous_mode) : continuous_mode_(continuous_mode) {}
  void setup() override;
  void loop() override;
  void dump_config() override;

  /// Returns volts for `config_bits` (multiplexer, gain and data rate fields of the config register).
  float request_measurement(uint16_t config_bits, uint8_t conversion_delay_ms);

#ifdef USE_ESP32
  /// Perform multiple consecutive measurements on the same channel (for RMS calculations)
  /// Holds a non-blocking lock during the burst to prevent channel switching
  /// @param multiplexer The ADC channel to measure
  /// @param gain The gain setting
  /// @param resolution The resolution (12 or 16 bits)
  /// @param samplerate The sample rate
  /// @param sample_callback Called for each sample with the voltage value
  /// @param num_samples Number of samples to take
  /// @return true if burst completed successfully, false if lock couldn't be acquired
  bool do_burst_measurement(ADS1115Multiplexer multiplexer, ADS1115Gain gain, ADS1115Resolution resolution,
                            ADS1115Samplerate samplerate, std::function<void(float)> sample_callback,
                            uint16_t num_samples);
#endif

 protected:
  float do_measurement_(ADS1115Multiplexer multiplexer, ADS1115Gain gain, ADS1115Resolution resolution,
                        ADS1115Samplerate samplerate);

  uint16_t prev_config_{0};
  const bool continuous_mode_;

#ifdef USE_ESP32
  // Non-blocking mutex for burst measurements
  SemaphoreHandle_t burst_mutex_{nullptr};
#endif
};

}  // namespace esphome::ads1115
