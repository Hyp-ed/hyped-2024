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
  try {
    auto request = chip_.prepare_request()
                     .set_consumer("hyped")
                     .add_line_settings(pin_,
                                        gpiod::line_settings()
                                          .set_direction(gpiod::line::direction::OUTPUT)
                                          .set_bias(gpiod::line::bias::PULL_UP))
                     .do_request();
    gpiod::line::value read_result = request.get_value(pin_);
    if (read_result == gpiod::line::value::ACTIVE) {
      logger_.log(core::LogLevel::kDebug, "Read high from GPIO %d", pin_);
      return core::DigitalSignal::kHigh;
    }
    logger_.log(core::LogLevel::kDebug, "Read low from GPIO %d", pin_);
    return core::DigitalSignal::kLow;
  } catch (const std::exception &e) {
    logger_.log(core::LogLevel::kFatal, "Error reading from GPIO %d: %s", pin_, e.what());
    return std::nullopt;
  }
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
  try {
    auto request = chip_.prepare_request()
                     .set_consumer("hyped")
                     .add_line_settings(
                       pin_, gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT))
                     .do_request();
    if (state == core::DigitalSignal::kLow) {
      logger_.log(core::LogLevel::kDebug, "Writing low to GPIO %d", pin_);
      request.set_value(pin_, gpiod::line::value::INACTIVE);
    }
    if (state == core::DigitalSignal::kHigh) {
      logger_.log(core::LogLevel::kDebug, "Writing high to GPIO %d", pin_);
      request.set_value(pin_, gpiod::line::value::ACTIVE);
    }
    return core::Result::kSuccess;
  } catch (const std::exception &e) {
    logger_.log(core::LogLevel::kFatal, "Error writing to GPIO %d: %s", pin_, e.what());
    return core::Result::kFailure;
  }
}

HardwareGpio::HardwareGpio(core::ILogger &log) : logger_(log), chip_(kGpioChipName)
{
}

std::optional<std::shared_ptr<IGpioReader>> HardwareGpio::getReader(const std::uint8_t pin,
                                                                    const Edge edge)
{
  return std::make_shared<HardwareGpioReader>(logger_, chip_, pin);
}

std::optional<std::shared_ptr<IGpioWriter>> HardwareGpio::getWriter(const std::uint8_t pin,
                                                                    const Edge edge)
{
  return std::make_shared<HardwareGpioWriter>(logger_, chip_, pin);
}

}  // namespace hyped::io
