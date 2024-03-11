#include "Relay.hpp"

namespace hyped::sensors {

std::optional<Relay> Relay::create(core::ILogger& logger,
                                    std::shared_ptr<io::IGpio> gpio,
                                    const std::uint8_t new_pin) {
    const auto reader = gpio->getReader(new_pin, io::Edge::kNone);
    if (!reader) {
        logger.log(core::LogLevel::kFatal, "Failed to create Relay instance");
        return std::nullopt;
    }
    logger.log(core::LogLevel::kDebug, "Successfully created Relay instance");
    return Relay(logger, *reader);
}

    // Convert DigitalSignal to a string
    const std::uint8_t signal_value = static_cast<std::uint8_t>(state); 
    char write_buffer[2];
    snprintf(write_buffer, sizeof(write_buffer), "%d", signal_value);
    // Write the value to the file
    const ssize_t write_result = ::write(write_file_descriptor_, write_buffer, sizeof(write_buffer));
    if (write_result != sizeof(write_buffer)) {
        logger_.log(core::LogLevel::kFatal, "Failed to write GPIO value");
        return core::Result Relay::close();
    }
    logger_.log(core::LogLevel::kDebug, "Wrote %d to GPIO", signal_value);
    return core::Result Relay::open() {
    // set relay to be open by writing the GPIO signal if needed
};

Relay::~Relay() {
}

bool RelayWriter::Write() {
    core::DigitalSignal result = gpio_writer_->write(); 
        return result;
}

}  // namespace hyped::sensors



#include "Relay.hpp"

namespace hyped::sensors {

std::optional<Relay> Relay::create(core::ILogger& logger,
                                    std::shared_ptr<io::IGpio> gpio,
                                    const std::uint8_t new_pin) {
    const auto reader = gpio->getReader(new_pin, io::Edge::kNone);
    if (!reader) {
        logger.log(core::LogLevel::kFatal, "Failed to create Relay instance");
        return std::nullopt;
    }
    logger.log(core::LogLevel::kDebug, "Successfully created Relay instance");
    return Relay(logger, *reader);
}

core::Result Relay::open() {
    // Implement relay open logic here
    // For example, set the GPIO pin to HIGH or ON
    // You can write to the GPIO value file as shown in your original code
    // ...

    // Return success or error result
    return core::Result::kSuccess;
}

core::Result Relay::close() {
    // Implement relay close logic here
    // For example, set the GPIO pin to LOW or OFF
    // You can write to the GPIO value file as shown in your original code
    // ...

    // Return success or error result
    return core::Result::kSuccess;
}

Relay::~Relay() {
    // Clean up any resources if needed
}

bool RelayWriter::Write() {
    // Assuming gpio_writer_ is an instance of your GPIO writer class
    core::DigitalSignal result = gpio_writer_->write();
    return result;  // Return true if successful, false otherwise
}

}  // namespace hyped::sensors

