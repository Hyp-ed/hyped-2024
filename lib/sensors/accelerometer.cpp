#include "accelerometer.hpp"

namespace hyped::sensors {

std::optional<Accelerometer> Accelerometer::create(core::ILogger &logger,
                                                   std::shared_ptr<io::II2c> i2c,
                                                   const std::uint8_t channel,
                                                   const std::uint8_t device_address)
{
  if (device_address != kDefaultAccelerometerAddress
      && device_address != kAlternativeAccelerometerAddress) {
    logger.log(core::LogLevel::kFatal, "Invalid device address for accelerometer");
    return std::nullopt;
  }
  // check we are communicating with the correct sensor
  const auto device_id = i2c->readByte(device_address, kDeviceIdAddress);
  if (!device_id) {
    logger.log(core::LogLevel::kFatal, "Failed to read the accelerometer device ID");
    return std::nullopt;
  }
  if (*device_id != kExpectedDeviceIdValue) {
    logger.log(core::LogLevel::kFatal, "Failure, mismatched device ID for accelerometer");
    return std::nullopt;
  }
  const auto ctrl1_result = i2c->writeByteToRegister(device_address, kCtrl1Address, kCtrl1Value);
  if (ctrl1_result == core::Result::kFailure) { return std::nullopt; };
  const auto ctrl2_result = i2c->writeByteToRegister(device_address, kCtrl2Address, kCtrl2Value);
  if (ctrl2_result == core::Result::kFailure) { return std::nullopt; };
  const auto ctrl6_result = i2c->writeByteToRegister(device_address, kCtrl6Address, kCtrl6Value);
  if (ctrl6_result == core::Result::kFailure) { return std::nullopt; };
  return Accelerometer(logger, i2c, channel, device_address);
}

Accelerometer::Accelerometer(core::ILogger &logger,
                             std::shared_ptr<io::II2c> i2c,
                             const std::uint8_t channel,
                             const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c),
      channel_(channel),
      device_address_(device_address)
{
}

Accelerometer::~Accelerometer()
{
}

std::optional<core::Result> Accelerometer::isValueReady()
{
  // check to see if the values are ready to be read
  const auto data_ready = i2c_->readByte(device_address_, kDataReady);
  if (!data_ready) {
    logger_.log(core::LogLevel::kFatal, "Failed to read acceleration data");
    return std::nullopt;
  }
  if (*data_ready % 2 == 0) {
    logger_.log(core::LogLevel::kWarn, "Failed to read acceleration data as it is not ready");
    return core::Result::kFailure;
  }
  return core::Result::kSuccess;
}

// TODOLater: current settings of the accelerometer make it read in +-16g but with high noise. Check
// whether other configuration could be better
std::optional<core::RawAccelerationData> Accelerometer::read()
{
  const auto result_x = getRawAcceleration(core::Axis::kX);
  if (!result_x) { return std::nullopt; }
  const std::int32_t x_acceleration = getAccelerationFromRawValue(*result_x);
  const auto result_y               = getRawAcceleration(core::Axis::kY);
  if (!result_y) { return std::nullopt; }
  const std::int32_t y_acceleration = getAccelerationFromRawValue(*result_y);
  const auto result_z               = getRawAcceleration(core::Axis::kZ);
  if (!result_z) { return std::nullopt; }
  const std::int32_t z_acceleration = getAccelerationFromRawValue(*result_z);
  core::WallClock time;
  // TODOLater (sensors): add a check for active sensor. Maybe remove optional return?
  const std::optional<core::RawAccelerationData> acceleration_three_axis{
    std::in_place, x_acceleration, y_acceleration, z_acceleration, time.now(), true};
  logger_.log(core::LogLevel::kDebug, "Successfully read accelerometer data");
  return acceleration_three_axis;
}

std::uint8_t Accelerometer::getChannel() const
{
  return channel_;
}

std::optional<std::int16_t> Accelerometer::getRawAcceleration(const core::Axis axis)
{
  setRegisterAddressFromAxis(axis);
  const auto low_byte = i2c_->readByte(device_address_, low_byte_address_);
  if (!low_byte) {
    logger_.log(core::LogLevel::kFatal,
                "Failed to read the low byte for acceleration along the %s",
                kAxisLabels[static_cast<std::size_t>(axis)]);
    return std::nullopt;
  }
  const auto high_byte = i2c_->readByte(device_address_, high_byte_address_);
  if (!high_byte) {
    logger_.log(core::LogLevel::kFatal,
                "Failed to read the high byte for acceleration along the %s",
                kAxisLabels[static_cast<std::size_t>(axis)]);
    return std::nullopt;
  }
  return static_cast<std::int16_t>((*high_byte << 8) | *low_byte);
}

std::int32_t Accelerometer::getAccelerationFromRawValue(const std::int16_t rawAcceleration)
{
  // scaling as per datasheet
  return (static_cast<std::int32_t>(rawAcceleration) * 488) / 1000;
}

void Accelerometer::setRegisterAddressFromAxis(const core::Axis axis)
{
  switch (axis) {
    case core::Axis::kX:
      low_byte_address_  = kXOutLow;
      high_byte_address_ = kXOutHigh;
      break;
    case core::Axis::kY:
      low_byte_address_  = kYOutLow;
      high_byte_address_ = kYOutHigh;
      break;
    case core::Axis::kZ:
      low_byte_address_  = kZOutLow;
      high_byte_address_ = kZOutHigh;
      break;
  }
}

}  // namespace hyped::sensors