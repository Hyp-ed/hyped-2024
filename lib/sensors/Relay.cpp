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
    // set relay to be open by writing the GPIO signal
};

Relay::~Relay() {
}


}  // namespace hyped::sensors

