#include "keyence.hpp"

#include <utility>

namespace hyped::sensors {

std::optional<std::shared_ptr<sensors::Keyence>> Keyence::create(
  core::ILogger &logger, const std::shared_ptr<io::IGpio> &gpio, const std::uint8_t pin)
{
  auto reader = gpio->getReader(pin);
  if (!reader) {
    logger.log(core::LogLevel::kFatal, "Failed to create GPIO reader for pin %d", pin);
    return std::nullopt;
  }
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

core::Result Keyence::updateStripeCount()
{
  const auto optional_signal = gpio_reader_->read();
  if (!optional_signal) {
    logger_.log(core::LogLevel::kFatal, "Failed to read GPIO");
    return core::Result::kFailure;
  }
  const auto signal = *optional_signal;
  if (signal == core::DigitalSignal::kHigh && last_signal_ == core::DigitalSignal::kLow) {
    ++stripe_count_;
    logger_.log(core::LogLevel::kDebug, "Stripe count increased to %d", stripe_count_);
  };
  last_signal_ = signal;
  return core::Result::kSuccess;
}

}  // namespace hyped::sensors
