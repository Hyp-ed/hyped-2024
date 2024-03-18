#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {

class Relay {
public:
    static std::optional<Relay> create(core::ILogger& logger,
                                       std::shared_ptr<io::IGpio> gpio,
                                       const std::uint8_t new_pin);

    ~Relay();

    core::Result open() {
        // Implement relay open logic here eg. set the GPIO pin to HIGH
        // write to the GPIO value file
        // replace 'pin' with the actual pin number

        if (gpio_writer_->set_pin_value(pin, true)) {
            logger_.INFO("Relay", "Relay opened successfully (Pin: %d)", pin);
            return core::Result::kSuccess;
        } else {
            logger_.ERROR("Relay", "Failed to open relay (Pin: %d)", pin);
            return core::Result::kFailure;
        }
    }

    core::Result close() {
        // Implement relay close logic here eg. set the GPIO pin to LOW
        // replace 'pin' with the actual pin number

        if (gpio_writer_->set_pin_value(pin, false)) {
            logger_.INFO("Relay", "Relay closed successfully (Pin: %d)", pin);
            return core::Result::kSuccess;
        } else {
            logger_.ERROR("Relay", "Failed to close relay (Pin: %d)", pin);
            return core::Result::kFailure;
        }
    }

private:
    Relay(core::ILogger& logger, std::shared_ptr<io::IGpioWriter> gpio_writer)
        : logger_(logger), gpio_writer_(gpio_writer) {
        // Initialize relay instance here
    }


    core::ILogger& logger_;
    std::shared_ptr<io::IGpioWriter> gpio_writer_;
    std::uint8_t pin;  // Replace with actual pin number
};

}  // namespace hyped::sensors
