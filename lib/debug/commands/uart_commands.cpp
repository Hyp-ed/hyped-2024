#include "uart_commands.hpp"

namespace hyped::debug {

core::Result UartCommands::addCommands(core::ILogger &logger, std::shared_ptr<Repl> repl)
{
  {
    const auto uart_read_command_name        = "uart read";
    const auto uart_read_command_description = "Read from a UART bus";
    const auto uart_read_command_usage       = "uart read <bus> <baud_rate> <bits_per_byte>";
    const auto uart_read_command_handler     = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 3) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto uart_bus      = static_cast<io::UartBus>(std::stoi(args[0]));
      const auto baud_rate     = static_cast<io::UartBaudRate>(std::stoi(args[1]));
      const auto bits_per_byte = static_cast<io::UartBitsPerByte>(std::stoi(args[2]));
      const auto optional_uart = repl->getUart(uart_bus, baud_rate, bits_per_byte);
      if (!optional_uart) {
        logger.log(core::LogLevel::kFatal, "Failed to get UART bus");
        return;
      }
      const auto uart = std::move(*optional_uart);
      std::uint8_t read_buffer[255];
      const core::Result result = uart->readBytes(read_buffer, 255);
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to read from UART bus %d", uart_bus);
        return;
      }
      logger.log(core::LogLevel::kDebug, "UART value from bus %d: %s", uart_bus, read_buffer);
    };
    auto uart_read_command = std::make_unique<Command>(uart_read_command_name,
                                                       uart_read_command_description,
                                                       uart_read_command_usage,
                                                       uart_read_command_handler);
    repl->addCommand(std::move(uart_read_command));
  }
  {
    const auto uart_write_command_name        = "uart write";
    const auto uart_write_command_description = "Write to a UART bus";
    const auto uart_write_command_usage   = "uart write <bus> <baud_rate> <bits_per_byte> <data>";
    const auto uart_write_command_handler = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 4) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto uart_bus      = static_cast<io::UartBus>(std::stoi(args[0]));
      const auto baud_rate     = static_cast<io::UartBaudRate>(std::stoi(args[1]));
      const auto bits_per_byte = static_cast<io::UartBitsPerByte>(std::stoi(args[2]));
      const auto optional_uart = repl->getUart(uart_bus, baud_rate, bits_per_byte);
      if (!optional_uart) {
        logger.log(core::LogLevel::kFatal, "Failed to get UART bus");
        return;
      }
      const auto uart           = std::move(*optional_uart);
      std::string data          = args[1];
      const core::Result result = uart->sendBytes(data.c_str(), data.length());
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to write to UART bus: %d", uart_bus);
        return;
      }
      logger.log(core::LogLevel::kDebug, "Successfully wrote to UART bus %d", uart_bus);
    };
    auto uart_write_command = std::make_unique<Command>(uart_write_command_name,
                                                        uart_write_command_description,
                                                        uart_write_command_usage,
                                                        uart_write_command_handler);
    repl->addCommand(std::move(uart_write_command));
  }

  return core::Result::kSuccess;
}

}  // namespace hyped::debug
