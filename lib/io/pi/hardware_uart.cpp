#include "hardware_uart.hpp"

#include <fcntl.h>
#include <unistd.h>

namespace hyped::io {

std::optional<std::shared_ptr<Uart>> Uart::create(core::ILogger &logger,
                                                  const UartBus bus,
                                                  const UartBaudRate baud_rate,
                                                  const UartBitsPerByte bits_per_byte)
{
  char path[15];  // up to "/dev/ttyO5"
  snprintf(path, sizeof(path), "/dev/ttyO%d", static_cast<std::uint8_t>(bus));
  const int file_descriptor = open(path, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (file_descriptor < 0) {
    logger.log(core::LogLevel::kFatal,
               "Failed to open UART file descriptor, could not create UART instance");
    return std::nullopt;
  }
  const std::uint32_t baud_mask = static_cast<std::uint32_t>(baud_rate);
  if (!baud_mask) {
    logger.log(core::LogLevel::kFatal,
               "Failed to set invalid baudrate, could not create UART instance");
    return std::nullopt;
  }
  const std::uint32_t bits_per_byte_mask = static_cast<std::uint32_t>(bits_per_byte);
  if (!bits_per_byte_mask) {
    logger.log(core::LogLevel::kFatal,
               "Failed to set invalid number of bits per byte, could not create UART instance");
    return std::nullopt;
  }
  const core::Result configuration_result
    = configureFileForOperation(logger, file_descriptor, baud_mask, bits_per_byte_mask);
  if (configuration_result == core::Result::kFailure) {
    logger.log(core::LogLevel::kFatal,
               "Failed to configure UART file, could not create UART instance");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully created UART instance");
  return std::make_shared<Uart>(logger, file_descriptor);
}

core::Result Uart::configureFileForOperation(core::ILogger &logger,
                                             const int file_descriptor,
                                             const std::uint32_t baud_mask,
                                             const std::uint32_t bits_per_byte_mask)
{
  struct termios tty;
  // resetting termios to remove any unintended configuration settings
  bzero(&tty, sizeof(tty));
  // exact setting descriptions available here
  // https://www.mkssoftware.com/docs/man5/struct_termios.5.asp
  tty.c_cflag                    = baud_mask | bits_per_byte_mask | CLOCAL | CREAD;
  tty.c_iflag                    = IGNPAR | ICRNL | IGNCR;
  tty.c_oflag                    = 0;
  tty.c_lflag                    = 0;
  tty.c_cc[VTIME]                = 0;
  tty.c_cc[VMIN]                 = 0;
  const int termios_flush_result = tcflush(file_descriptor, TCIFLUSH);
  if (termios_flush_result < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to flush UART file for non-transmitted data");
    return core::Result::kFailure;
  }
  const int termios_config_result = tcsetattr(file_descriptor, TCSANOW, &tty);
  if (termios_config_result < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to configure UART file for operation");
    return core::Result::kFailure;
  }
  logger.log(core::LogLevel::kDebug, "Successfully configured UART file for operation");
  return core::Result::kSuccess;
}

Uart::Uart(core::ILogger &logger, const int file_descriptor)
    : logger_(logger),
      file_descriptor_(file_descriptor)
{
}

Uart::~Uart()
{
  close(file_descriptor_);
}

core::Result Uart::sendBytes(const char *tx, const std::uint8_t length)
{
  const ssize_t write_result = write(file_descriptor_, tx, length);
  if (write_result != length) {
    logger_.log(core::LogLevel::kFatal, "Failed to write the desired number of bytes to UART");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully wrote %d bytes to UART", length);
  return core::Result::kSuccess;
}

core::Result Uart::readBytes(unsigned char *rx, const std::uint8_t length)
{
  const ssize_t read_result = read(file_descriptor_, rx, length);
  if (read_result != length) {
    logger_.log(core::LogLevel::kFatal, "Failed to read the desired number of bytes from UART");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully read %d bytes over UART", length);
  return core::Result::kFailure;
}

}  // namespace hyped::io