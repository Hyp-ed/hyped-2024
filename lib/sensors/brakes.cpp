brakeSensor cpp


#include "brakes.hpp"

namespace hyped::sensors {

    std::optional<Brakes> Brakes::create(core::ILogger &logger,
                                         std::shared_ptr<io::IGpio> gpio,
                                         const std::uint8_t new_pin)
    {
        const auto reader = gpio->getReader(new_pin, io::Edge::kNone);
        if (!reader) {
            logger.log(core::LogLevel::kFatal, "Failed to create Brakes instance");
            return std::nullopt;
        }
        logger.log(core::LogLevel::kDebug, "Successfully created Brakes instance");
        return Brakes(logger, *reader);
    }

    Brakes::Brakes(core::ILogger &logger, std::shared_ptr<io::IGpioReader> gpio_reader)
            : gpio_reader_(gpio_reader),
              logger_(logger),
              on(), off()
    {
    }

    Brakes::~Brakes()
    {
    }

    bool Brakes::highLow() {
        core::DigitalSignal signal = gpio_reader_->read();
//    std::int8_t currentDistance = ;
        if (signal <= core::DigitalSignal::kHigh) {
            return true;
        }
        return false;
    }
}  // namespace hyped::sensors
