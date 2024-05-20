#include "optical_flow.hpp"

#include <utility>

namespace hyped::sensors {

std::optional<std::shared_ptr<OpticalFlow>> OpticalFlow::create(
  core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi)
{
  // check we are communicating with the correct sensor
  const std::uint8_t device_id[1] = {0};
  const auto result               = spi->read(kDeviceIdAddress, device_id, 1);
  if (result == core::Result::kFailure) {
    logger.log(core::LogLevel::kFatal, "Failed to read the optical flow device");
    return std::nullopt;
  }
  if (*device_id != kExpectedDeviceIdValue) {
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
  std::uint8_t x_low[1] = {0};
  const auto low_result = spi_->read(kXLowAddress, x_low, 1);
  if (low_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the low byte of the x delta");
    return std::nullopt;
  }
  std::uint8_t x_high[1] = {0};
  const auto high_result = spi_->read(kXHighAddress, x_high, 1);
  if (high_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the high byte of the x delta");
    return std::nullopt;
  }
  return static_cast<std::int16_t>(x_high[0]) << 8 | x_low[0];
}

std::optional<std::uint16_t> OpticalFlow::getDeltaY() const
{
  std::uint8_t y_low[1] = {0};
  const auto low_result = spi_->read(kYLowAddress, y_low, 1);
  if (low_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the low byte of the y delta");
    return std::nullopt;
  }
  std::uint8_t y_high[1] = {0};
  const auto high_result = spi_->read(kYHighAddress, y_high, 1);
  if (high_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the high byte of the y delta");
    return std::nullopt;
  }
  return (static_cast<std::int16_t>(y_high[0]) << 8) | y_low[0];
}

}  // namespace hyped::sensors
