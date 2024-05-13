#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {
class Keyence {
 public:
  static std::optional<std::shared_ptr<Keyence>> create(core::ILogger &logger,
                                                        const std::shared_ptr<io::IGpio> &gpio,
                                                        const std::uint8_t new_pin);

  Keyence(core::ILogger &logger, std::shared_ptr<io::IGpioReader> gpio_reader);

  std::uint8_t getStripeCount() const;

  void updateStripeCount();

 private:
  std::uint8_t stripe_count_;
  std::shared_ptr<io::IGpioReader> gpio_reader_;
  core::ILogger &logger_;
  core::DigitalSignal last_signal_;
};

}  // namespace hyped::sensors
