#pragma once

#include "i2c_sensors.hpp"

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

enum class temperatureAddress { k7F = 0x7F, k38 = 0x38, k3C = 0x3C, k3E = 0x3E };

class Temperature : public II2cMuxSensor<std::int16_t> {
 public:
  static std::optional<std::shared_ptr<Temperature>> create(
    core::ILogger &logger,
    const std::shared_ptr<io::II2c> &i2c,
    const std::uint8_t channel,
    const temperatureAddress device_address);

  Temperature(core::ILogger &logger,
              std::shared_ptr<io::II2c> i2c,
              const std::uint8_t channel,
              const std::uint8_t device_address);

  /*
   * @brief Checks if the temperature sensor is ready to be read
   * @return kSuccess if the sensor is ready to be read,
   *         kFailure if the sensor is not ready to be read,
   *         nullopt if there was an error reading the status register
   */
  std::optional<core::Result> checkStatus();

  /**
   * @brief Reads the temperature from the sensor
   */
  std::optional<std::int16_t> read() override;

  std::uint8_t getChannel() const override;

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t channel_;
  const std::uint8_t device_address_;

  // Register addresses/values taken from the datasheet
  static constexpr std::uint8_t kCtrl                = 0x04;
  static constexpr std::uint8_t kDataTemperatureHigh = 0x07;
  static constexpr std::uint8_t kDataTemperatureLow  = 0x06;
  static constexpr std::uint8_t kStatus              = 0x05;
  // The values to check the status of the temperature sensor from the 0x05 register
  static constexpr std::uint8_t kBusy                       = 0x01;
  static constexpr std::uint8_t kTemperatureOverUpperLimit  = 0x02;
  static constexpr std::uint8_t kTemperatureUnderLowerLimit = 0x04;
  // Sets the sensor to continuous mode, sets IF_ADD_INC, and sets sampling rate to 200Hz
  static constexpr std::uint8_t kConfigurationSetting  = 0x3c;
  static constexpr core::Float kTemperatureScaleFactor = 0.01;
};

}  // namespace hyped::sensors
