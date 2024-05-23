#include "optical_flow.hpp"

#include <utility>

#include "core/logger.hpp"

namespace hyped::sensors {

std::optional<std::shared_ptr<OpticalFlow>> OpticalFlow::create(
  core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi, Rotation rotation)
{
  // check we are communicating with the correct sensor
  auto optional_device_id = spi->read(kDeviceIdAddress, 1);
  if (!optional_device_id) {
    logger.log(core::LogLevel::kFatal, "Failed to read the optical flow device ID");
    return std::nullopt;
  }
  const auto device_id = *optional_device_id;
  if (device_id[0] != kExpectedDeviceIdValue) {
    logger.log(core::LogLevel::kFatal, "Failure, mismatched device ID for optical flow sensor");
    return std::nullopt;
  }
  const auto power_up_result = spi->write(kPowerUpResetAddress, {kPowerUpResetValue});
  if (power_up_result != core::Result::kSuccess) {
    logger.log(core::LogLevel::kFatal, "Failed to power up reset the optical flow sensor");
    return std::nullopt;
  }
  const auto rotation_result = setRotation(logger, spi, rotation);
  if (rotation_result != core::Result::kSuccess) {
    logger.log(core::LogLevel::kFatal, "Failed to set the rotation of the optical flow sensor");
    return std::nullopt;
  }
  const auto magic_result = doMagic(logger, spi);
  if (magic_result != core::Result::kSuccess) {
    logger.log(core::LogLevel::kFatal, "Failed to send magic numbers to the optical flow sensor");
    return std::nullopt;
  }
  return std::make_shared<OpticalFlow>(logger, spi);
}

OpticalFlow::OpticalFlow(core::ILogger &logger, std::shared_ptr<io::ISpi> spi)
    : logger_(logger),
      spi_(std::move(spi))
{
}

static constexpr std::uint8_t kMotionBurst = 0x16;

std::optional<std::uint16_t> OpticalFlow::getDeltaX() const
{
  auto data = spi_->read(kMotionBurst, 5);
  if (!data) {
    logger_.log(core::LogLevel::kFatal, "Failed to read motion burst data");
    return std::nullopt;
  }
  logger_.log(core::LogLevel::kDebug,
              "Motion burst data: %d %d %d %d %d",
              (*data)[0],
              (*data)[1],
              (*data)[2],
              (*data)[3],
              (*data)[4]);
  return std::nullopt;
  //  auto optional_x_low = spi_->read(kXLowAddress, 1);
  //  if (!optional_x_low) {
  //    logger_.log(core::LogLevel::kFatal, "Failed to read the low byte of the x delta");
  //    return std::nullopt;
  //  }
  //  const auto x_low     = *optional_x_low;
  //  auto optional_x_high = spi_->read(kXHighAddress, 1);
  //  if (!optional_x_high) {
  //    logger_.log(core::LogLevel::kFatal, "Failed to read the high byte of the x delta");
  //    return std::nullopt;
  //  }
  //  const auto x_high = *optional_x_high;
  //  return static_cast<std::int16_t>(x_high[0]) << 8 | x_low[0];
}

std::optional<std::uint16_t> OpticalFlow::getDeltaY() const
{
  auto optional_y_low = spi_->read(kYLowAddress, 1);
  if (!optional_y_low) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the low byte of the y delta");
    return std::nullopt;
  }
  const auto y_low     = *optional_y_low;
  auto optional_y_high = spi_->read(kYHighAddress, 1);
  if (!optional_y_high) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the high byte of the y delta");
    return std::nullopt;
  }
  const auto y_high = *optional_y_high;
  logger_.log(core::LogLevel::kDebug, "y_low: %d, y_high: %d", y_low[0], y_high[0]);
  return (static_cast<std::int16_t>(y_high[0]) << 8) | y_low[0];
}

core::Result OpticalFlow::doMagic(core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi)
{
  for (const auto &[address, value] : kMagicNumbers) {
    const std::vector<std::uint8_t> tx = {value};
    const auto result                  = spi->write(address, tx);
    if (result != core::Result::kSuccess) {
      logger.log(core::LogLevel::kFatal, "Failed to write magic number to %d", address);
      return result;
    }
  }
  return core::Result::kSuccess;
}

core::Result OpticalFlow::setRotation(core::ILogger &logger,
                                      const std::shared_ptr<io::ISpi> &spi,
                                      Rotation rotation)
{
  std::uint8_t orientation = 0;
  switch (rotation) {
    case Rotation::kNone:
      orientation = kInvertX | kInvertY | kSwapXY;
      break;
    case Rotation::kClockwise90:
      orientation = kInvertY;
      break;
    case Rotation::kClockwise180:
      orientation = 0;
      break;
    case Rotation::kClockwise270:
      orientation = kInvertX;
      break;
  }
  const std::vector<std::uint8_t> tx = {orientation};
  return spi->write(kOrientationAddress, tx);
}

}  // namespace hyped::sensors
