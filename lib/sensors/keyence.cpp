#include "keyence.hpp"

namespace hyped::sensors {

std::optional<Keyence> Keyence::create(core::ILogger &logger,
                                       std::shared_ptr<io::IGpio> gpio,
                                       const std::uint8_t new_pin)
{
  const auto reader = gpio->getReader(new_pin);
  if (!reader) {
    logger.log(core::LogLevel::kFatal, "Failed to create Keyence instance");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully created Keyence instance");
  return Keyence(logger, *reader);
}

Keyence::Keyence(core::ILogger &logger, std::shared_ptr<io::IGpioReader> gpio_reader)
    : gpio_reader_(gpio_reader),
      logger_(logger)
{
}

Keyence::~Keyence()
{
}

std::uint8_t Keyence::getStripeCount()
{
  return stripe_count_;
}

void Keyence::updateStripeCount()
{
  if (gpio_reader_->read() == core::DigitalSignal::kHigh) {
    ++stripe_count_;
    logger_.log(core::LogLevel::kDebug, "Stripe count increased to %d", stripe_count_);
  };
}

}  // namespace hyped::sensors