#include <iostream>
#include<.clang-format>
#include<i2c_sensors.hpp>
#include "tof.hpp"

namespace hyped::sensor{
std::optional<TimeOfFlight> TimeOfFlight::create(core::ILogger &logger,
                                                       std::shared_ptr<io::II2c> i2c,
                                                       const std::uint8_t device_address)
{ {
  return TimeOfFlight(logger, i2c, device_address);
}

TimeOfFlight::TimeOfFlight    (core::ILogger &logger,
                                 std::shared_ptr<io::II2c> i2c,
                                 const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c),
      device_address_(device_address)
{
{
  if (device_address != kDefaultVL6180XAddress) {
    logger.log(core::LogLevel::kFatal, "Invalid device address for VL6180X sensor");
    return std::nullopt;
  }
  const auto write_result = i2c->writeByteToRegister(device_address, kCtrl, kConfigurationSetting);
  if (write_result == core::Result::kFailure) {
    logger.log(
      core::LogLevel::kFatal, "Failed to configure VL6180X sensor at channel %d", channel);
    return std::nullopt;
  }
  logger.log(
    core::LogLevel::kDebug, "Successful to configure VL6180X sensor at channel %d", channel);
  return VL6180X(logger, i2c, channel, device_address);
}




}