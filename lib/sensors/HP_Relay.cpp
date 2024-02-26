#include "HPRelay.hpp"

namespace hyped::sensors {

HPRelay::HPRelay(core::ILogger &log) : logger_(log)
{
}

std::optional<std::shared_ptr<IHPRelayWriter>> HPRelay::getWriter(const std::uint8_t pin,
                                                                    const Edge edge)
{
  const core::Result initialise_result = initialisePin(pin, edge, Direction::kOut);
  if (initialise_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to initialise GPIO %d", pin);
    return std::nullopt;
  }
  const int write_file_descriptor = getFileDescriptor(pin, Direction::kOut);
  if (write_file_descriptor < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to get file descriptor for GPIO %d", pin);
    return std::nullopt;
  }
  return std::make_shared<HardwareGpioWriter>(logger_, write_file_descriptor);
}

HPRelayWriter::~HPRelayWriter()
{
  close(write_file_descriptor_);
}

core::Result HPRelayWriter::write(const core::DigitalSignal state)
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

}  // namespace hyped::sensors