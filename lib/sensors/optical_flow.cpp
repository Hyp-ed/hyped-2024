#include "optical_flow.hpp"

namespace hyped::sensors {

std::optional<OpticalFlow> OpticalFlow::create(core::ILogger &logger,
                                               std::shared_ptr<io::ISpi> spi,
                                               const std::uint8_t channel)
{
  // check we are communicating with the correct sensor
  const uint8_t *device_id;
  spi->read(kDeviceIdAddress, device_id, 1);
  if (!device_id) {
    logger.log(core::LogLevel::kFatal, "Failed to read the optical flow device");
    return std::nullopt;
  }
  if (*device_id != kExpectedDeviceIdValue) {
    logger.log(core::LogLevel::kFatal, "Failure, mismatched device ID for optical flow sensor");
    return std::nullopt;
  }
  return OpticalFlow(logger, spi, channel);
}

OpticalFlow::OpticalFlow(core::ILogger &logger,
                         std::shared_ptr<io::ISpi> spi,
                         const std::uint8_t channel)
    : logger_(logger),
      spi_(spi),
      channel_(channel)
{
}

OpticalFlow::~OpticalFlow()
{
}

// inspired by Bitcraze_PMW3901 on github
// (https://github.com/bitcraze/Bitcraze_PMW3901/blob/master/src/Bitcraze_PMW3901.cpp)
std::uint8_t OpticalFlow::getDeltaX(std::shared_ptr<io::ISpi> spi)
{
  uint8_t *x_low;
  uint8_t *x_high;

  spi->read(kXLowAddress, x_low, 1);
  spi->read(kXHighAddress, x_high, 1);

  return static_cast<std::int16_t>((*x_high << 8) | *x_low);
}
std::uint8_t OpticalFlow::getDeltaY(std::shared_ptr<io::ISpi> spi)
{
  uint8_t *y_low;
  uint8_t *y_high;

  spi->read(kYLowAddress, y_low, 1);
  spi->read(kYHighAddress, y_high, 1);

  return static_cast<std::int16_t>((*y_high << 8) | *y_low);
}
}  // namespace hyped::sensors