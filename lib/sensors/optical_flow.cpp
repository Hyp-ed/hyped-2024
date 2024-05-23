#include "optical_flow.hpp"

#include <cmath>

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

std::optional<std::uint16_t> OpticalFlow::read()
{
  auto optional_data = spi_->read(kMotionBurst, 12);
  if (!optional_data) {
    logger_.log(core::LogLevel::kFatal, "Failed to read motion burst data");
    return std::nullopt;
  }
  const auto data          = *optional_data;
  const auto ready         = data[1] & 0x80;
  const auto x             = static_cast<std::uint16_t>((data[4] << 8) | data[3]);
  const auto y             = static_cast<std::uint16_t>((data[6] << 8) | data[5]);
  const auto quality       = data[7];
  const auto shutter_upper = data[11];
  logger_.log(core::LogLevel::kDebug,
              "Read data: ready %d, x %d, y %d, quality %d, shutter upper %d",
              ready,
              x,
              y,
              quality,
              shutter_upper);
  return std::sqrt(x * x + y * y);
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
