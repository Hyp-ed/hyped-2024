#include "hardware_gpio.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdint>

#include "core/types.hpp"

namespace hyped::io {

HardwareGpioReader::HardwareGpioReader(core::ILogger &logger,
                                       gpiod::chip &chip,
                                       const std::uint8_t pin)
    : logger_(logger),
      chip_(chip),
      pin_(pin)
{
}

std::optional<core::DigitalSignal> HardwareGpioReader::read()
{
  return std::nullopt;
}

HardwareGpioWriter::HardwareGpioWriter(core::ILogger &log,
                                       gpiod::chip &chip,
                                       const std::uint8_t pin)
    : logger_(log),
      chip_(chip),
      pin_(pin)
{
}

core::Result HardwareGpioWriter::write(const core::DigitalSignal state)
{
  return core::Result::kFailure;
}

HardwareGpio::HardwareGpio(core::ILogger &log) : logger_(log), chip_(kGpioChipName)
{
}

std::optional<core::DigitalSignal> HardwareGpio::read(const std::uint8_t pin)
{
  try {
    gpiod::line_request request
      = chip_.prepare_request()
          .set_consumer("hyped")
          .add_line_settings(pin,
                             gpiod::line_settings().set_direction(gpiod::line::direction::INPUT))
          .do_request();
    gpiod::line::value read_result = request.get_value(pin);
    if (read_result == gpiod::line::value::ACTIVE) {
      logger_.log(core::LogLevel::kDebug, "Read high from GPIO %d", pin);
      return core::DigitalSignal::kHigh;
    }
    logger_.log(core::LogLevel::kDebug, "Read low from GPIO %d", pin);
    request.release();
    return core::DigitalSignal::kLow;
  } catch (const std::exception &e) {
    logger_.log(core::LogLevel::kFatal, "Error reading from GPIO %d: %s", pin, e.what());
    return std::nullopt;
  }
}

core::Result HardwareGpio::write(const std::uint8_t pin, const core::DigitalSignal state)
{
  try {
    auto request = chip_.prepare_request()
                     .set_consumer("hyped")
                     .add_line_settings(
                       pin, gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT))
                     .do_request();
    if (state == core::DigitalSignal::kLow) {
      logger_.log(core::LogLevel::kDebug, "Writing low to GPIO %d", pin);
      request.set_value(pin, gpiod::line::value::INACTIVE);
    }
    if (state == core::DigitalSignal::kHigh) {
      logger_.log(core::LogLevel::kDebug, "Writing high to GPIO %d", pin);
      request.set_value(pin, gpiod::line::value::ACTIVE);
    }
    request.release();
    return core::Result::kSuccess;
  } catch (const std::exception &e) {
    logger_.log(core::LogLevel::kFatal, "Error writing to GPIO %d: %s", pin, e.what());
    return core::Result::kFailure;
  }
}

}  // namespace hyped::io
