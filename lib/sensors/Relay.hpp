#pragma once

#include <cstdint>
#include <memory>

#include <optional>
#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {

class Relay {
 public:
        static std::optional <Relay> create(core::ILogger &logger,
                                             std::shared_ptr <io::IGpio> gpio,
                                             const std::uint8_t new_pin);

        ~Relay(); 

    private:
       Relay(core::ILogger &logger, std::shared_ptr <io::IGpioWriter> gpio_writer);
};


   //  bool open();
     bool Write();  

   private:
     RelayWriter(core::ILogger &logger_, const int write_file_descriptor_);
};

}  // namespace hyped::sensors


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
        // Implement relay open logic here
        // For example, set the GPIO pin to HIGH or ON
        // You can write to the GPIO value file as shown in your original code
        // ...

        // Return success or error result
        return core::Result::kSuccess;
    }

    core::Result close() {
        // Implement relay close logic here
        // For example, set the GPIO pin to LOW or OFF
        // You can write to the GPIO value file as shown in your original code
        // ...

        // Return success or error result
        return core::Result::kSuccess;
    }

private:
    Relay(core::ILogger& logger, std::shared_ptr<io::IGpioWriter> gpio_writer);
};

class RelayWriter {
public:
    RelayWriter(core::ILogger& logger_, const int write_file_descriptor_);

    bool Write() {
        // Assuming gpio_writer_ is an instance of your GPIO writer class
        core::DigitalSignal result = gpio_writer_->write();
        return result;  // Return true if successful, false otherwise
    }

private:
    // Add any necessary members for RelayWriter
};

}  // namespace hyped::sensors
