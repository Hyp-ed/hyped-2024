#include "keyence.hpp"

#include <utility>

namespace hyped::sensors {

std::optional<std::shared_ptr<Keyence>> Keyence::create(core::ILogger &logger,
                                                        const std::shared_ptr<io::IGpio> &gpio,
                                                        const std::uint8_t pin)
{
  const auto reader = gpio->getReader(pin, io::Edge::kNone);
  if (!reader) {
    logger.log(core::LogLevel::kFatal, "Failed to create Keyence instance");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully created Keyence instance");
  return std::make_shared<Keyence>(logger, *reader);
}

Keyence::Keyence(core::ILogger &logger, std::shared_ptr<io::IGpioReader> gpio_reader)
    : gpio_reader_(std::move(gpio_reader)),
      logger_(logger),
      stripe_count_(0),
      last_signal_(core::DigitalSignal::kLow)
{
}

std::uint8_t Keyence::getStripeCount() const
{
  return stripe_count_;
}

void Keyence::updateStripeCount()
{
  const auto optional_signal = gpio_reader_->read();
  if (!optional_signal) {
    logger_.log(core::LogLevel::kFatal, "Failed to read from keyence sensor");
    return;
  }
  const auto signal = *optional_signal;
  if (signal == core::DigitalSignal::kHigh && last_signal_ == core::DigitalSignal::kLow) {
    ++stripe_count_;
    logger_.log(core::LogLevel::kDebug, "Stripe count increased to %d", stripe_count_);
  };
  last_signal_ = signal;
}

}  // namespace hyped::sensors
