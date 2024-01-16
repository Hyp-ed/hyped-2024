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

  // TODOLater - Implement
  std::optional<core::Result> checkStatus();

  std::uint8_t getChannel();

 private:
  TimeOfFlight(core::ILogger &logger,
               std::shared_ptr<io::II2c> i2c,
               const std::uint8_t channel,
               const std::uint8_t device_address);

 private:
  // TODOLater - Confirm these addresses are correct
  // Register addresses/values taken from the VL6180X datasheet
  static constexpr std::uint8_t kCtrl                 = 0x00;
  static constexpr std::uint8_t kDataHigh             = 0x00;
  static constexpr std::uint8_t kDataLow              = 0x00;
  static constexpr std::uint8_t kStatus               = 0x00;
  static constexpr std::uint8_t kBusy                 = 0x00;
  static constexpr std::uint8_t kConfigurationSetting = 0x00;

  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t channel_;
  const std::uint8_t device_address_;
};

}  // namespace hyped::sensors
