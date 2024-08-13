#include "brakes.hpp"

namespace hyped::sensors {

std::optional<Brakes> Brakes::create(core::ILogger &logger,
                                     std::shared_ptr<io::IGpio> gpio,
                                     const std::uint8_t pin)
{
  const auto reader = gpio->getReader(pin, io::Edge::kNone);
  if (!reader) {
    logger.log(core::LogLevel::kFatal, "Failed to create Brakes instance");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully created Brakes instance");
  return Brakes(logger, *reader);
}

Brakes::Brakes(core::ILogger &logger, std::shared_ptr<io::IGpioReader> gpio_reader)
    : gpio_reader_(gpio_reader),
      logger_(logger),
      pin_()
{
}

Brakes::~Brakes()
{
}

std::optional<bool> Brakes::isClamped()
{
  const auto optional_reading = gpio_reader_->read();
  if (!optional_reading) { return std::nullopt; }
  const auto reading = *optional_reading;

  if (reading == core::DigitalSignal::kHigh) { return false; }
  return true;
}
}  // namespace hyped::sensors