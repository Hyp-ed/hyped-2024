#pragma once

#include "i2c_sensors.hpp"

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {
constexpr std::uint8_t kDefaultTimeOfFlightAddress = 0x29;

// TODOLater - Implement II2cMuxSensor
class TimeOfFlight {
 public:
  static std::optional<TimeOfFlight> create(core::ILogger &logger,
                                            std::shared_ptr<io::II2c> i2c,
                                            const std::uint8_t channel,
                                            const std::uint8_t device_address
                                            = kDefaultTimeOfFlightAddress);

  ~TimeOfFlight();

  std::optional<std::uint8_t> getStatus();

  std::uint8_t getChannel();

  // TODOLater - Implement any other functions of ToF sensor

 private:
  TimeOfFlight(core::ILogger &logger,
               std::shared_ptr<io::II2c> i2c,
               const std::uint8_t channel,
               const std::uint8_t device_address);

  /**
   * @brief Set registers in device for default use
   * @return kSuccess if successful; kFailure otherwise
   * @note See ST Application Note AN4545 Section 1.3 for details
   */
  core::Result initialise();

  /**
   * @brief Reads the measured range in Single-Shot mode
   * @note Implementation based on ST Application Note AN4545
   */
  std::optional<std::uint8_t> getRange();

 private:
  // TODOLater - Confirm these addresses are correct
  // Register addresses/values taken from the VL6180X datasheet
  static constexpr std::uint8_t kCtrl                 = 0x00;
  static constexpr std::uint8_t kDataHigh             = 0x00;
  static constexpr std::uint8_t kDataLow              = 0x00;
  static constexpr std::uint8_t kStatus               = 0x00;
  static constexpr std::uint8_t kBusy                 = 0x00;
  static constexpr std::uint8_t kConfigurationSetting = 0x00;

  static constexpr std::uint8_t kSystemFreshOutOfReset = 0x016;

  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t channel_;
  const std::uint8_t device_address_;
};

}  // namespace hyped::sensors
