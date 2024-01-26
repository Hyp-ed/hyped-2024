#include "time_of_flight.hpp"

namespace hyped::sensors {
std::optional<TimeOfFlight> TimeOfFlight::create(core::ILogger &logger,
                                                 std::shared_ptr<io::II2c> i2c,
                                                 const std::uint8_t channel,
                                                 const std::uint8_t device_address)
{
  if (device_address != kDefaultTimeOfFlightAddress) {
    logger.log(core::LogLevel::kFatal, "Invalid device address for time of flight sensor");
    return std::nullopt;
  }
  const auto write_result = i2c->writeByteToRegister(device_address, kCtrl, kConfigurationSetting);
  if (write_result == core::Result::kFailure) {
    logger.log(
      core::LogLevel::kFatal, "Failed to configure time of flight sensor at channel %d", channel);
    return std::nullopt;
  }
  logger.log(
    core::LogLevel::kDebug, "Successfully configured time of flight sensor at channel %d", channel);
  return TimeOfFlight(logger, i2c, channel, device_address);
}

TimeOfFlight::TimeOfFlight(core::ILogger &logger,
                           std::shared_ptr<io::II2c> i2c,
                           const std::uint8_t channel,
                           const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c),
      channel_(channel),
      device_address_(device_address)
{
}

core::Result TimeOfFlight::initialise()
{
  return core::Result::kSuccess;
}

std::optional<std::uint8_t> TimeOfFlight::getRange()
{
  // Start measurement in Single-Shot mode
  const auto start_status = i2c_->writeByteToRegister(device_address_, 0x018, 0x01);
  if (start_status == core::Result::kFailure) { return std::nullopt; }

  // Check the status
  auto status = i2c_->readByte(device_address_, 0x04f);
  if (!status) { return std::nullopt; }
  auto range_status = *status & 0x07;

  // Wait for new measurement ready status
  while (range_status != 0x04) {
    status = i2c_->readByte(device_address_, 0x04f);
    if (!status) { return std::nullopt; }
    range_status = *status & 0x07;
  }

  const auto range = i2c_->readByte(device_address_, 0x062);

  // Clear interrupts
  const auto interrupt_reset_status = i2c_->writeByteToRegister(device_address_, 0x015, 0x07);
  if (interrupt_reset_status == core::Result::kFailure) { return std::nullopt; }

  return range;
};

std::optional<std::uint8_t> TimeOfFlight::getStatus()
{
  const auto status = i2c_->readByte(device_address_, kStatus);
  if (!status) {
    logger_.log(core::LogLevel::kFatal, "Failed to read time of flight status");
    return std::nullopt;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully read time of flight status");
  return status;
};

std::uint8_t TimeOfFlight::getChannel()
{
  return channel_;
};

}  // namespace hyped::sensors