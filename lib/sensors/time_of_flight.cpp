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
  auto time_of_flight          = TimeOfFlight(logger, i2c, channel, device_address);
  const auto initialise_result = time_of_flight.initialise();
  if (initialise_result == core::Result::kFailure) {
    logger.log(
      core::LogLevel::kFatal, "Failed to configure time of flight sensor at channel %d", channel);
    return std::nullopt;
  }
  logger.log(
    core::LogLevel::kDebug, "Successfully configured time of flight sensor at channel %d", channel);
  return time_of_flight;
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

TimeOfFlight::~TimeOfFlight()
{
}

// TODOLater - Implement writing to 16-bit I2C registers first!
core::Result TimeOfFlight::initialise()
{
  const auto status = i2c_->readByte(device_address_, kSystemFreshOutOfReset);
  if (!status) { return core::Result::kFailure; }
  if (*status == 1) {
    // TODOLater - Check core::Result after every writeByteToRegister?
    // Register names are not public - See ST Application Note AN4545 Section 9
    // i2c_->writeByteToRegister(device_address_, 0x0207, 0x01);
    // i2c_->writeByteToRegister(device_address_, 0x0208, 0x01);

    i2c_->writeByteToRegister(device_address_, array_return_addresses[0], 0x00);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[1], 0xfd);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[2], 0x01);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[3], 0x03);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[4], 0x02);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[5], 0x01);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[6], 0x03);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[7], 0x02);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[8], 0x05);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[9], 0xce);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[10], 0x03);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[11], 0xf8);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[12], 0x00);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[13], 0x3c);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[14], 0x00);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[15], 0x3c);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[16], 0x09);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[17], 0x09);
    // i2c_->writeByteToRegister(device_address_, 0x0198, 0x01);
    // i2c_->writeByteToRegister(device_address_, 0x01b0, 0x17);
    // i2c_->writeByteToRegister(device_address_, 0x01ad, 0x00);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[18], 0x05);
    // i2c_->writeByteToRegister(device_address_, 0x0100, 0x05);
    // i2c_->writeByteToRegister(device_address_, 0x0199, 0x05);
    // i2c_->writeByteToRegister(device_address_, 0x01a6, 0x1b);
    // i2c_->writeByteToRegister(device_address_, 0x01ac, 0x3e);
    // i2c_->writeByteToRegister(device_address_, 0x01a7, 0x1f);
    i2c_->writeByteToRegister(device_address_, array_return_addresses[19], 0x00);

    i2c_->writeByteToRegister(device_address_, kSystemFreshOutOfReset, 0);

    // Recommended : Public registers
    i2c_->writeByteToRegister(device_address_, kSystemModeGpioOne, 0x10);
    // i2c_->writeByteToRegister(device_address_, kReadoutSamplingPeriod, 0x30);
    i2c_->writeByteToRegister(device_address_, kSysAlsAnalogueGain, 0x46);
    i2c_->writeByteToRegister(device_address_, kSysRangeVhvRepeatRate, 0xff);
    i2c_->writeByteToRegister(device_address_, kSysAlsIntegrationPeriod, 0x63);
    i2c_->writeByteToRegister(device_address_, kSysRangeVhvRecalibrate, 0x01);

    // Optional: Public registers
    i2c_->writeByteToRegister(device_address_, kSysRangeIntermeasurementPeriod, 0x09);
    i2c_->writeByteToRegister(device_address_, kSysAlsIntermeasurementPeriod, 0x31);
    i2c_->writeByteToRegister(device_address_, kSystemInterruptConfigGpio, 0x24);
  }
  return core::Result::kSuccess;
}

std::optional<std::uint8_t> TimeOfFlight::getRangeSingleShot()
{
  if (checkSensorMode(kModeSingleShot) == core::Result::kFailure) { return std::nullopt; }
  return getRange();
};

std::optional<std::uint8_t> TimeOfFlight::getRangeContinuous()
{
  if (checkSensorMode(kModeContinuous) == core::Result::kFailure) { return std::nullopt; }
  return getRange();
};

core::Result TimeOfFlight::checkSensorMode(std::uint8_t mode_value)
{
  const auto status = i2c_->readByte(device_address_, kSysRangeStart);
  if (!status) {
    logger_.log(core::LogLevel::kFatal, "Failed to check time of flight mode");
    return core::Result::kFailure;
  }
  if (status != mode_value) {
    const auto start_status
      = i2c_->writeByteToRegister(device_address_, kSysRangeStart, mode_value);
    if (start_status == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to set time of flight mode");
      return core::Result::kFailure;
    }
  }
  return core::Result::kSuccess;
};

std::optional<std::uint8_t> TimeOfFlight::getRange()
{
  // Check the status
  auto status = i2c_->readByte(device_address_, kResultInterruptStatusGpio);
  if (!status) {
    logger_.log(core::LogLevel::kFatal, "Failed to get time of flight interrupt status");
    return std::nullopt;
  }
  auto range_status = *status & 0b111;

  // Wait for new measurement ready status
  while (range_status != 0x04) {
    status = i2c_->readByte(device_address_, kResultInterruptStatusGpio);
    if (!status) {
      logger_.log(core::LogLevel::kFatal, "Failed to get time of flight interrupt status");
      return std::nullopt;
    }
    range_status = *status & 0b111;
  }

  const auto range = i2c_->readByte(device_address_, kResultRangeVal);
  if (!range) {
    logger_.log(core::LogLevel::kFatal, "Failed to read range from time of flight sensor");
    return std::nullopt;
  }

  // Clear interrupts
  const auto interrupt_reset_status
    = i2c_->writeByteToRegister(device_address_, kSystemInterruptClear, 0b111);
  if (interrupt_reset_status == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to clear interrupts");
    return std::nullopt;
  }

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