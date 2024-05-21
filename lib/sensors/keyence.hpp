#pragma once

#include <cstdint>
#include <memory>

#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {
class Keyence {
 public:
  static std::optional<std::shared_ptr<sensors::Keyence>> create(
    core::ILogger &logger, const std::shared_ptr<io::IGpio> &gpio, const std::uint8_t pin);

  Keyence(core::ILogger &logger,
          std::shared_ptr<io::IGpioReader> gpio_reader,
          const std::uint8_t pin);

  std::uint8_t getStripeCount() const;
  core::Result updateStripeCount();

 private:
  std::uint8_t stripe_count_;
  std::shared_ptr<io::IGpioReader> gpio_reader_;
  core::ILogger &logger_;
  core::DigitalSignal last_signal_;
  const std::uint8_t pin_;
};

}  // namespace hyped::sensors
