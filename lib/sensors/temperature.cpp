#include "temperature.hpp"

#include <utility>

namespace hyped::sensors {

std::optional<std::shared_ptr<Temperature>> Temperature::create(
  core::ILogger &logger,
  const std::shared_ptr<io::II2c> &i2c,
  const temperatureAddress device_address)
{
  const auto device_address_int = static_cast<std::uint8_t>(device_address);
  const auto write_result
    = i2c->writeByteToRegister(device_address_int, kCtrl, kConfigurationSetting);
  if (write_result == core::Result::kFailure) {
    logger.log(core::LogLevel::kFatal, "Failed to configure temperature sensor");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully configured temperature sensor");
  return std::make_shared<Temperature>(logger, i2c, device_address_int);
}

Temperature::Temperature(core::ILogger &logger,
                         std::shared_ptr<io::II2c> i2c,
                         const std::uint8_t device_address)
    : logger_(logger),
      i2c_(std::move(i2c)),
      device_address_(device_address)
{
}

std::optional<core::Result> Temperature::checkStatus()
{
  const auto status_check_result = i2c_->readByte(device_address_, kStatus);
  if (!status_check_result) {
    logger_.log(core::LogLevel::kFatal, "Failed to read temperature sensor status");
    return std::nullopt;
  }
  if (*status_check_result == kBusy) {
    logger_.log(core::LogLevel::kWarn,
                "Failed to read, temperature sensor is not ready to be read");
    return core::Result::kFailure;
  }
  if (*status_check_result == kTemperatureOverUpperLimit) {
    logger_.log(core::LogLevel::kFatal, "Failed to read, temperature is above upper limit");
    return std::nullopt;
  }
  if (*status_check_result == kTemperatureUnderLowerLimit) {
    logger_.log(core::LogLevel::kFatal, "Failed to read, temperature is below lower limit");
    return std::nullopt;
  }
  return core::Result::kSuccess;
}

std::optional<std::int16_t> Temperature::read()
{
  const auto temperature_high_byte = i2c_->readByte(device_address_, kDataTemperatureHigh);
  if (!temperature_high_byte) {
    logger_.log(core::LogLevel::kFatal, "Failed to read high byte for temperature");
    return std::nullopt;
  }
  const auto temperature_low_byte = i2c_->readByte(device_address_, kDataTemperatureLow);
  if (!temperature_low_byte) {
    logger_.log(core::LogLevel::kFatal, "Failed to read low byte for temperature");
    return std::nullopt;
  }
  const std::int16_t temperature = (*temperature_high_byte << 8) | *temperature_low_byte;
  logger_.log(core::LogLevel::kDebug, "Successfully read from temperature sensor");
  // Scaling temperature as per the datasheet
  return static_cast<std::int16_t>(temperature * kTemperatureScaleFactor);
}

}  // namespace hyped::sensors
