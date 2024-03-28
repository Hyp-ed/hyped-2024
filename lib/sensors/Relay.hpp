
#pragma once

#include <cstdint>
#include <memory>

#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {

class Relay {
 public:
  // Public constructor
  Relay(core::ILogger &logger,
        std::shared_ptr<io::IGpioWriter> gpio_writer,
        const std::uint8_t &new_pin)
      : logger_(logger),
        gpio_writer_(gpio_writer),
        pin(new_pin)
  {
    // Initialize relay instance here
    gpio_writer_->set_pin_direction(pin, io::GpioDirection::kOut);
  }

  // Public static create function
  static std::optional<Relay> create(core::ILogger &logger,
                                     std::shared_ptr<io::IGpioWriter> gpio_writer,
                                     const std::uint8_t new_pin)
  {
    return Relay(logger, gpio_writer, new_pin);
  }

  Relay::~Relay() { logger_.INFO("Relay", "Relay destroyed (Pin: %d)", pin); }

  core::Result open()
  {
    // Implement relay open logic here (e.g., set the GPIO pin to HIGH)
    if (gpio_writer_->set_pin_value(pin, true)) {
      logger_.INFO("Relay", "Relay opened successfully (Pin: %d)", pin);
      return core::Result::kSuccess;
    } else {
      logger_.ERROR("Relay", "Failed to open relay (Pin: %d)", pin);
      return core::Result::kFailure;
    }
  }

  core::Result close()
  {
    // Implement relay close logic here (e.g., set the GPIO pin to LOW)
    if (gpio_writer_->set_pin_value(pin, false)) {
      logger_.INFO("Relay", "Relay closed successfully (Pin: %d)", pin);
      return core::Result::kSuccess;
    } else {
      logger_.ERROR("Relay", "Failed to close relay (Pin: %d)", pin);
      return core::Result::kFailure;
    }
  }

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::IGpioWriter> gpio_writer_;
  std::uint8_t pin;  // Replace with the actual pin number
};

}  // namespace hyped::sensors
