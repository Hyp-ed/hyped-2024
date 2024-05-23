#include "optical_flow.hpp"

#include <utility>

#include "core/logger.hpp"

namespace hyped::sensors {

std::optional<std::shared_ptr<OpticalFlow>> OpticalFlow::create(
  core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi)
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
  return std::make_shared<OpticalFlow>(logger, spi);
}

OpticalFlow::OpticalFlow(core::ILogger &logger, std::shared_ptr<io::ISpi> spi)
    : logger_(logger),
      spi_(std::move(spi))
{
}

std::optional<std::uint16_t> OpticalFlow::getDeltaX() const
{
  auto optional_x_low = spi_->read(kXLowAddress, 1);
  if (!optional_x_low) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the low byte of the x delta");
    return std::nullopt;
  }
  const auto x_low     = *optional_x_low;
  auto optional_x_high = spi_->read(kXHighAddress, 1);
  if (!optional_x_high) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the high byte of the x delta");
    return std::nullopt;
  }
  const auto x_high = *optional_x_high;
  return static_cast<std::int16_t>(x_high[0]) << 8 | x_low[0];
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

}  // namespace hyped::sensors
