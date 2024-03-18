#include "led_driver.hpp"

namespace hyped::sensors {

std::optional<LedDriver> LedDriver::create(core::ILogger &logger, std::shared_ptr<io::II2c> i2c)
{
  auto ledDriver = LedDriver(logger, i2c);
  if (auto init_result = ledDriver.initialise(); init_result == core::Result::kFailure) {
    logger.log(core::LogLevel::kFatal, "Failed to initialise LED driver");
    return std::nullopt;
  }

  logger.log(core::LogLevel::kDebug, "Successfully initialised LED driver");

  return ledDriver;
}

LedDriver::LedDriver(core::ILogger &logger, std::shared_ptr<io::II2c> i2c)
    : logger_(logger),
      i2c_(i2c)
{
}

LedDriver::~LedDriver()
{
}

core::Result LedDriver::initialise()
{
  const auto write_led_control_result
    = i2c_->writeByteToRegister(device_address_, kLEDControlRegister, 0x00);

  if (write_led_control_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to initialise LED driver");
    return core::Result::kFailure;
  }

  return core::Result::kSuccess;
}

core::Result LedDriver::setColour(const std::uint8_t channel,
                                  const std::uint8_t red,
                                  const std::uint8_t green,
                                  const std::uint8_t blue)
{
  const auto write_red_result
    = i2c_->writeByteToRegister(device_address_, kColorRegisterBase + channel, red);
  const auto write_green_result
    = i2c_->writeByteToRegister(device_address_, kColorRegisterBase + channel + 1, green);
  const auto write_blue_result
    = i2c_->writeByteToRegister(device_address_, kColorRegisterBase + channel + 2, blue);

  if (write_red_result == core::Result::kFailure || write_green_result == core::Result::kFailure
      || write_blue_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to set color for LED channel %d", channel);
    return core::Result::kFailure;
  }

  return core::Result::kSuccess;
}

core::Result LedDriver::setIntensity(std::uint8_t channel, std::uint8_t intensity)
{
  const auto write_intensity_result
    = i2c_->writeByteToRegister(device_address_, kBrightnessRegisterBase + channel, intensity);

  if (write_intensity_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to set intensity for LED channel %d", channel);
    return core::Result::kFailure;
  }

  return core::Result::kSuccess;
}

core::Result LedDriver::reset()
{
  const auto write_reset_result = i2c_->writeByteToRegister(device_address_, kResetRegister, 0x00);

  if (write_reset_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to perform a reset for the LED driver");
    return core::Result::kFailure;
  }

  return core::Result::kSuccess;
}

}  // namespace hyped::sensors
