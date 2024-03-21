#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {
class Brakes {
 public:
  static std::optional<Brakes> create(core::ILogger &logger,
                                      std::shared_ptr<io::IGpio> gpio,
                                      const std::uint8_t new_pin);

  ~Brakes();

  std::optional<bool> isClamped();

 private:
  Brakes(core::ILogger &logger, std::shared_ptr<io::IGpioReader> gpio_reader);

 private:
  const std::uint8_t pin_;
  std::shared_ptr<io::IGpioReader> gpio_reader_;
  core::ILogger &logger_;
};
}  // namespace hyped::sensors