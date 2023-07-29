#include "hardware_gpio.hpp"

#include <fcntl.h>
#include <unistd.h>

namespace hyped::io {

HardwareGpioReader::HardwareGpioReader(core::ILogger &log, const int read_file_descriptor)
    : logger_(log),
      read_file_descriptor_(read_file_descriptor)
{
}

HardwareGpioReader::~HardwareGpioReader()
{
  close(read_file_descriptor_);
}

std::optional<core::DigitalSignal> HardwareGpioReader::read()
{
  // Read the value from the file
  char read_buffer[2];
  const off_t offset = lseek(read_file_descriptor_, 0, SEEK_SET);
  if (offset != 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to reset file offset");
    return std::nullopt;
  }
  const ssize_t read_result = ::read(read_file_descriptor_, read_buffer, sizeof(read_buffer));
  if (read_result != sizeof(read_buffer)) {
    logger_.log(core::LogLevel::kFatal, "Failed to read GPIO value");
    return std::nullopt;
  }
  // Convert the read value to a DigitalSignal
  const int value = std::atoi(read_buffer);
  if (value == 0) {
    return core::DigitalSignal::kLow;
  } else if (value == 1) {
    return core::DigitalSignal::kHigh;
  } else {
    logger_.log(core::LogLevel::kFatal, "Invalid GPIO value read");
    return std::nullopt;
  }
}

HardwareGpioWriter::HardwareGpioWriter(core::ILogger &log, const int write_file_descriptor)
    : logger_(log),
      write_file_descriptor_(write_file_descriptor)
{
}

HardwareGpioWriter::~HardwareGpioWriter()
{
  close(write_file_descriptor_);
}

core::Result HardwareGpioWriter::write(const core::DigitalSignal state)
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

HardwareGpio::HardwareGpio(core::ILogger &log) : logger_(log)
{
}

std::optional<std::shared_ptr<IGpioReader>> HardwareGpio::getReader(const std::uint8_t pin,
                                                                    const Edge edge)
{
  const core::Result initialise_result = initialisePin(pin, edge, Direction::kIn);
  if (initialise_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to initialise GPIO %d", pin);
    return std::nullopt;
  }
  const int read_file_descriptor = getFileDescriptor(pin, Direction::kIn);
  if (read_file_descriptor < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to get file descriptor for GPIO %d", pin);
    return std::nullopt;
  }
  return std::make_shared<HardwareGpioReader>(logger_, read_file_descriptor);
}

std::optional<std::shared_ptr<IGpioWriter>> HardwareGpio::getWriter(const std::uint8_t pin,
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

core::Result HardwareGpio::exportPin(const std::uint8_t pin)
{
  const int export_file_descriptor = open("/sys/class/gpio/export", O_WRONLY);
  if (export_file_descriptor < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to open GPIO export file");
    return core::Result::kFailure;
  }
  char write_buffer[4];
  snprintf(write_buffer, sizeof(write_buffer), "%d", pin);
  const ssize_t write_result = write(export_file_descriptor, write_buffer, sizeof(write_buffer));
  close(export_file_descriptor);
  if (write_result != sizeof(write_buffer)) {
    logger_.log(core::LogLevel::kFatal, "Failed to export GPIO %d", pin);
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully exported GPIO %d", pin);
  return core::Result::kSuccess;
}

core::Result HardwareGpio::initialisePin(const std::uint8_t pin,
                                         const Edge edge,
                                         const Direction direction)
{
  // First check if the pin is already exported, and export it if not
  char file_path_buffer[50];
  snprintf(file_path_buffer, sizeof(file_path_buffer), "/sys/class/gpio/gpio%d", pin);
  const int access_result = access(file_path_buffer, F_OK);  // Check if the file exists
  if (access_result < 0) {
    logger_.log(core::LogLevel::kDebug, "GPIO %d not exported, exporting", pin);
    const core::Result export_result = exportPin(pin);
    if (export_result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to export GPIO %d while initialising", pin);
      return core::Result::kFailure;
    }
  }
  // Then set the direction
  snprintf(file_path_buffer, sizeof(file_path_buffer), "/sys/class/gpio/gpio%d/direction", pin);
  const int direction_file_descriptor = open(file_path_buffer, O_WRONLY);
  if (direction_file_descriptor < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to open GPIO direction file");
    return core::Result::kFailure;
  }
  const std::string direction_string = getDirectionString(direction);
  const ssize_t direction_write_result
    = write(direction_file_descriptor, direction_string.c_str(), direction_string.size() + 1);
  close(direction_file_descriptor);
  if (direction_write_result < 0) {
    logger_.log(core::LogLevel::kFatal,
                "Failed to set GPIO %d, direction error %d, errno %d",
                pin,
                direction_write_result,
                errno);
    return core::Result::kFailure;
  }
  // Finally set the edge
  snprintf(file_path_buffer, sizeof(file_path_buffer), "/sys/class/gpio/gpio%d/edge", pin);
  const int edge_file_descriptor = open(file_path_buffer, O_WRONLY);
  if (edge_file_descriptor < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to open GPIO edge file");
    return core::Result::kFailure;
  }
  const std::string edge_string = getEdgeString(edge);
  const ssize_t edge_write_result
    = write(edge_file_descriptor, edge_string.c_str(), edge_string.size() + 1);
  close(edge_file_descriptor);
  if (edge_write_result < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to set the edge for GPIO %d, errno %d", pin, errno);
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully initialised GPIO %d", pin);
  return core::Result::kSuccess;
}

int HardwareGpio::getFileDescriptor(const std::uint8_t pin, const Direction direction)
{
  // Set up the file path
  char value_file_path[64];
  snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", pin);
  // Set up the file descriptor
  int file_descriptor;
  // The direction is used to determine whether the file descriptor is opened for reading or writing
  if (direction == Direction::kIn) {
    file_descriptor = open(value_file_path, O_RDONLY);
  } else {
    file_descriptor = open(value_file_path, O_WRONLY);
  }
  if (file_descriptor < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to open GPIO value file");
    return -1;
  }
  return file_descriptor;
}

const std::string HardwareGpio::getEdgeString(const Edge edge)
{
  switch (edge) {
    case Edge::kNone:
      return "none";
    case Edge::kRising:
      return "rising";
    case Edge::kFalling:
      return "falling";
    case Edge::kBoth:
      return "both";
    default:  // for compiler
      return "";
  }
}

const std::string HardwareGpio::getDirectionString(const Direction direction)
{
  switch (direction) {
    case Direction::kIn:
      return "in";
    case Direction::kOut:
      return "out";
    default:  // for compiler
      return "";
  }
}

}  // namespace hyped::io