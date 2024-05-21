#include "keyence.hpp"

#include <utility>

namespace hyped::sensors {

std::optional<std::shared_ptr<Keyence>> Keyence::create(core::ILogger &logger,
                                                        std::shared_ptr<io::HardwareGpio> gpio,
                                                        const std::uint8_t pin)
{
  logger.log(core::LogLevel::kDebug, "Successfully created Keyence instance");
  return std::make_shared<Keyence>(logger, gpio, pin);
}

Keyence::Keyence(core::ILogger &logger,
                 std::shared_ptr<io::HardwareGpio> gpio,
                 const std::uint8_t pin)
    : gpio_(std::move(gpio)),
      logger_(logger),
      stripe_count_(0),
      last_signal_(core::DigitalSignal::kLow),
      pin_(pin)
{
}

std::uint8_t Keyence::getStripeCount() const
{
  return stripe_count_;
}

void Keyence::updateStripeCount()
{
  const auto signal = gpio_->read(pin_);
  if (signal == core::DigitalSignal::kHigh && last_signal_ == core::DigitalSignal::kLow) {
    ++stripe_count_;
    logger_.log(core::LogLevel::kDebug, "Stripe count increased to %d", stripe_count_);
  };
  last_signal_ = signal;
}

}  // namespace hyped::sensors
