#include "HPRelay.hpp"

namespace hyped::sensors {


    std::optional<Relay>Relay::create(core::ILogger &logger,
                                            std::shared_ptr<io::IGpio> gpio,
                                            const std::uint8_t new_pin)

{
        const auto reader = gpio->getReader(new_pin, io::Edge::kNone);
        if (!reader) {
            logger.log(core::LogLevel::kFatal, "Failed to create Relay instance");
            return std::nullopt;
        }
        logger.log(core::LogLevel::kDebug, "Successfully created Relay instance");
        return Brakes(logger, *reader);

} 


Relay::~Relay()
{
}

std::optional<RelayWriter> RelayWriter::getWriter(const std::uint8_t pin,
                                                                    const Edge edge)

{
  // Convert DigitalSignal to a string
  const std::uint8_t signal_value = static_cast<std::uint8_t>(state);
  char write_buffer[2];
  snprintf(write_buffer, sizeof(write_buffer), "%d", signal_value);
  // Write the value to the file
  const ssize_t write_result = ::write(write_file_descriptor_, write_buffer, sizeof(write_buffer));
  if (write_result != sizeof(write_buffer)) {
    logger_.log(core::LogLevel::kFatal, "Failed to write GPIO value");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Wrote %d to GPIO", signal_value);
  return core::Result::kSuccess;
}

RelayWriter::~RelayWriter()
{
}

bool RelayWriter::Write() {
    core::DigitalSignal logger_.log = gpio_writer_ -> write();
    if(logger_.log(core::LogLevel::kDebug, "Wrote %d to GPIO", signal_value));
     return core::Result::kSuccess;
    } 
    return core::Result::kFailure;
    
}


}  // namespace hyped::sensors