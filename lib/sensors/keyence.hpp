#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "io/hardware_gpio.hpp"
#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {
class Keyence {
 public:
  static std::optional<std::shared_ptr<Keyence>> create(core::ILogger &logger,
                                                        std::shared_ptr<io::HardwareGpio> gpio,
                                                        const std::uint8_t pin);

  Keyence(core::ILogger &logger, std::shared_ptr<io::HardwareGpio> gpio, const std::uint8_t pin);

  std::uint8_t getStripeCount() const;

  void updateStripeCount();

 private:
  std::uint8_t stripe_count_;
  std::shared_ptr<io::HardwareGpio> gpio_;
  core::ILogger &logger_;
  core::DigitalSignal last_signal_;
  const std::uint8_t pin_;
};

}  // namespace hyped::sensors
