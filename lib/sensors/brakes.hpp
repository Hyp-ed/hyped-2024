#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/gpio.hpp>


namespace hyped::sensors {
    class Brakes {
    public:
        static std::optional <Brakes> create(core::ILogger &logger,
                                             std::shared_ptr <io::IGpio> gpio,
                                             const std::uint8_t new_pin);

        ~Brakes();

//  bool open();
        bool highLow();


    private:
        Brakes(core::ILogger &logger, std::shared_ptr <io::IGpioReader> gpio_reader);

    private:
        std::uint8_t pin_;
        std::uint8_t on; // if distance is closer, it is on 0 and 2
        std::uint8_t off; // if distance is further, it is off. 2 and 5
        std::shared_ptr <io::IGpioReader> gpio_reader_;
        core::ILogger &logger_;
    };
}