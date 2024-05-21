#include "hardware_gpio.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdint>

#include "core/types.hpp"

namespace hyped::io {

HardwareGpioReader::HardwareGpioReader(core::ILogger &logger,
                                       gpiod::line_request &request,
                                       const std::uint8_t pin)
    : logger_(logger),
      request_(request),
      pin_(pin)
{
}

HardwareGpioReader::~HardwareGpioReader()
{
  request_.release();
}

std::optional<core::DigitalSignal> HardwareGpioReader::read()
{
  try {
    gpiod::line::value read_result = request_.get_value(pin_);
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
                                       gpiod::line_request &request,
                                       const std::uint8_t pin)
    : logger_(log),
      request_(request),
      pin_(pin)
{
}

HardwareGpioWriter::~HardwareGpioWriter()
{
  request_.release();
}

core::Result HardwareGpioWriter::write(const core::DigitalSignal state)
{
  try {
    if (state == core::DigitalSignal::kLow) {
      logger_.log(core::LogLevel::kDebug, "Writing low to GPIO %d", pin_);
      request_.set_value(pin_, gpiod::line::value::INACTIVE);
    }
    if (state == core::DigitalSignal::kHigh) {
      logger_.log(core::LogLevel::kDebug, "Writing high to GPIO %d", pin_);
      request_.set_value(pin_, gpiod::line::value::ACTIVE);
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
  try {
    gpiod::line_request request
      = chip_.prepare_request()
          .set_consumer("hyped")
          .add_line_settings(pin,
                             gpiod::line_settings().set_direction(gpiod::line::direction::INPUT))
          .do_request();
    return std::make_shared<HardwareGpioReader>(logger_, request, pin);
  } catch (const std::exception &e) {
    logger_.log(core::LogLevel::kFatal, "Error creating GPIO reader for pin %d: %s", pin, e.what());
    return std::nullopt;
  }
}

std::optional<std::shared_ptr<IGpioWriter>> HardwareGpio::getWriter(const std::uint8_t pin,
                                                                    const Edge edge)
{
  try {
    gpiod::line_request request
      = chip_.prepare_request()
          .set_consumer("hyped")
          .add_line_settings(pin,
                             gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT))
          .do_request();
    return std::make_shared<HardwareGpioWriter>(logger_, request, pin);
  } catch (const std::exception &e) {
    logger_.log(core::LogLevel::kFatal, "Error creating GPIO reader for pin %d: %s", pin, e.what());
    return std::nullopt;
  }
}

}  // namespace hyped::io
