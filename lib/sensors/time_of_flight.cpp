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

// TODOLater - Implement writing to 16-bit I2C registers first!
core::Result TimeOfFlight::initialise()
{
  const auto status = i2c_->readByte(device_address_, kSystemFreshOutOfReset);
  if (!status) { return core::Result::kFailure; }
  if (*status == 1) {
    // TODOLater - Replace magic numbers with constants?
    // TODOLater - Check core::Result after every writeByteToRegister?
    // // Mandatory : private registers
    // i2c_->writeByteToRegister(device_address_, 0x0207, 0x01);
    // i2c_->writeByteToRegister(device_address_, 0x0208, 0x01);
    // i2c_->writeByteToRegister(device_address_, 0x0096, 0x00);
    // i2c_->writeByteToRegister(device_address_, 0x0097, 0xfd);
    // i2c_->writeByteToRegister(device_address_, 0x00e3, 0x01);
    // i2c_->writeByteToRegister(device_address_, 0x00e4, 0x03);
    // i2c_->writeByteToRegister(device_address_, 0x00e5, 0x02);
    // i2c_->writeByteToRegister(device_address_, 0x00e6, 0x01);
    // i2c_->writeByteToRegister(device_address_, 0x00e7, 0x03);
    // i2c_->writeByteToRegister(device_address_, 0x00f5, 0x02);
    // i2c_->writeByteToRegister(device_address_, 0x00d9, 0x05);
    // i2c_->writeByteToRegister(device_address_, 0x00db, 0xce);
    // i2c_->writeByteToRegister(device_address_, 0x00dc, 0x03);
    // i2c_->writeByteToRegister(device_address_, 0x00dd, 0xf8);
    // i2c_->writeByteToRegister(device_address_, 0x009f, 0x00);
    // i2c_->writeByteToRegister(device_address_, 0x00a3, 0x3c);
    // i2c_->writeByteToRegister(device_address_, 0x00b7, 0x00);
    // i2c_->writeByteToRegister(device_address_, 0x00bb, 0x3c);
    // i2c_->writeByteToRegister(device_address_, 0x00b2, 0x09);
    // i2c_->writeByteToRegister(device_address_, 0x00ca, 0x09);
    // i2c_->writeByteToRegister(device_address_, 0x0198, 0x01);
    // i2c_->writeByteToRegister(device_address_, 0x01b0, 0x17);
    // i2c_->writeByteToRegister(device_address_, 0x01ad, 0x00);
    // i2c_->writeByteToRegister(device_address_, 0x00ff, 0x05);
    // i2c_->writeByteToRegister(device_address_, 0x0100, 0x05);
    // i2c_->writeByteToRegister(device_address_, 0x0199, 0x05);
    // i2c_->writeByteToRegister(device_address_, 0x01a6, 0x1b);
    // i2c_->writeByteToRegister(device_address_, 0x01ac, 0x3e);
    // i2c_->writeByteToRegister(device_address_, 0x01a7, 0x1f);
    // i2c_->writeByteToRegister(device_address_, 0x0030, 0x00);

    i2c_->writeByteToRegister(device_address_, kSystemFreshOutOfReset, 0);

    // // Recommended : Public registers
    // i2c_->writeByteToRegister(device_address_, 0x0011, 0x10);
    // i2c_->writeByteToRegister(device_address_, 0x010a, 0x30);
    // i2c_->writeByteToRegister(device_address_, 0x003f, 0x46);
    // i2c_->writeByteToRegister(device_address_, 0x0031, 0xff);
    // i2c_->writeByteToRegister(device_address_, 0x0041, 0x63);
    // i2c_->writeByteToRegister(device_address_, 0x002e, 0x01);

    // // Optional: Public registers
    // i2c_->writeByteToRegister(device_address_, 0x001b, 0x09);
    // i2c_->writeByteToRegister(device_address_, 0x003e, 0x31);
    // i2c_->writeByteToRegister(device_address_, 0x0013, 0x24);
  }
  return core::Result::kSuccess;
}

std::optional<std::uint8_t> TimeOfFlight::getRange()
{
  // TODOLater - Replace magic numbers with constants
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