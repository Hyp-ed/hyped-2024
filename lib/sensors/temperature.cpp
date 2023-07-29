#include "temperature.hpp"

namespace hyped::sensors {

std::optional<Temperature> Temperature::create(core::ILogger &logger,
                                               std::shared_ptr<io::II2c> i2c,
                                               const std::uint8_t channel,
                                               const std::uint8_t device_address)
{
  if (device_address != kDefaultTemperatureAddress
      && device_address != kAlternativeTemperatureAddress) {
    logger.log(core::LogLevel::kFatal, "Invalid device address for temperature sensor");
    return std::nullopt;
  }
  const auto write_result = i2c->writeByteToRegister(device_address, kCtrl, kConfigurationSetting);
  if (write_result == core::Result::kFailure) {
    logger.log(
      core::LogLevel::kFatal, "Failed to configure temperature sensor at channel %d", channel);
    return std::nullopt;
  }
  logger.log(
    core::LogLevel::kDebug, "Successful to configure temperature sensor at channel %d", channel);
  return Temperature(logger, i2c, channel, device_address);
}

Temperature::Temperature(core::ILogger &logger,
                         std::shared_ptr<io::II2c> i2c,
                         const std::uint8_t channel,
                         const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c),
      channel_(channel),
      device_address_(device_address)
{
}

Temperature::~Temperature()
{
}

std::optional<core::Result> Temperature::checkStatus()
{
  const auto status_check_result = i2c_->readByte(device_address_, kStatus);
  if (!status_check_result) {
    logger_.log(
      core::LogLevel::kFatal, "Failed to read temperature sensor status at channel %d", channel_);
    return std::nullopt;
  }
  if (*status_check_result == kBusy) {
    logger_.log(core::LogLevel::kWarn,
                "Failed to read, temperature sensor is not ready to be read at channel %d",
                channel_);
    return core::Result::kFailure;
  }
  if (*status_check_result == kTemperatureOverUpperLimit) {
    logger_.log(core::LogLevel::kFatal,
                "Failed to read, temperature is above upper limit at channel %d",
                channel_);
    return std::nullopt;
  }
  if (*status_check_result == kTemperatureUnderLowerLimit) {
    logger_.log(core::LogLevel::kFatal,
                "Failed to read, temperature is below lower limit at channel %d",
                channel_);
    return std::nullopt;
  }
  return core::Result::kSuccess;
}

std::optional<std::int16_t> Temperature::read()
{
  const auto temperature_high_byte = i2c_->readByte(device_address_, kDataTemperatureHigh);
  if (!temperature_high_byte) {
    logger_.log(
      core::LogLevel::kFatal, "Failed to read high byte for temperature at channel %d", channel_);
    return std::nullopt;
  }
  const auto temperature_low_byte = i2c_->readByte(device_address_, kDataTemperatureLow);
  if (!temperature_low_byte) {
    logger_.log(
      core::LogLevel::kFatal, "Failed to read low byte for temperature at channel %d", channel_);
    return std::nullopt;
  }
  const std::int16_t temperature = ((*temperature_high_byte << 8) | *temperature_low_byte);
  logger_.log(
    core::LogLevel::kDebug, "Successfully read from temperature sensor at channel %d", channel_);
  // Scaling temperature as per the datasheet
  return static_cast<std::int16_t>(temperature * kTemperatureScaleFactor);
}

std::uint8_t Temperature::getChannel() const
{
  return channel_;
}

}  // namespace hyped::sensors