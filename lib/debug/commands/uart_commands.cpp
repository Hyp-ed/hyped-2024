#include "uart_commands.hpp"

namespace hyped::debug {
core::Result UartCommands::addCommands(core::ILogger &logger,
                                       std::shared_ptr<Repl> repl,
                                       toml::v3::node_view<toml::v3::node> config)
{
  const auto buses = config["buses"].as_array();
  if (!buses) {
    logger.log(core::LogLevel::kFatal, "No UART buses specified");
    return core::Result::kFailure;
  }
  for (const auto &bus : *buses) {
    const auto optional_bus = bus.value<std::uint8_t>();
    if (!optional_bus) {
      logger.log(core::LogLevel::kFatal, "Invalid UART bus");
      return core::Result::kFailure;
    }
    const auto bus_number = *optional_bus;
    const auto uart_bus   = static_cast<io::UartBus>(bus_number);
    const auto optional_uart
      = repl->getUart(uart_bus, io::UartBaudRate::kB38400, io::UartBitsPerByte::k8);
    if (!optional_uart) {
      logger.log(core::LogLevel::kFatal, "Error creating UART bus");
      return core::Result::kFailure;
    }
    const auto uart = std::move(*optional_uart);
    {
      const auto uart_read_command_name        = "uart " + std::to_string(bus_number) + " read";
      const auto uart_read_command_description = "Read from UART bus " + std::to_string(bus_number);
      const auto uart_read_command_handler     = [&logger, uart, bus_number] {
        std::uint8_t read_buffer[255];
        const core::Result result = uart->readBytes(read_buffer, 255);
        if (result == core::Result::kFailure) {
          logger.log(core::LogLevel::kFatal, "Failed to read from UART bus %d", bus_number);
          return;
        }
        logger.log(core::LogLevel::kDebug, "UART value from bus %d: %s", bus_number, read_buffer);
      };
      auto uart_read_command = std::make_unique<Command>(
        uart_read_command_name, uart_read_command_description, uart_read_command_handler);
      repl->addCommand(std::move(uart_read_command));
    }
    {
      const auto uart_write_command_name        = "uart " + std::to_string(bus_number) + " write";
      const auto uart_write_command_description = "Write to UART bus " + std::to_string(bus_number);
      const auto uart_write_command_handler     = [&logger, uart, bus_number]() {
        std::string data;
        std::cout << "Data: ";
        std::getline(std::cin, data);
        if (data.length() > 255) {
          logger.log(core::LogLevel::kFatal, "Data too long for UART bus: %d", bus_number);
          return;
        }
        const core::Result result = uart->sendBytes(data.c_str(), data.length());
        if (result == core::Result::kFailure) {
          logger.log(core::LogLevel::kFatal, "Failed to write to UART bus: %d", bus_number);
          return;
        }
        logger.log(core::LogLevel::kDebug, "Successfully wrote to UART bus %d", bus_number);
      };
      auto uart_write_command = std::make_unique<Command>(
        uart_write_command_name, uart_write_command_description, uart_write_command_handler);
      repl->addCommand(std::move(uart_write_command));
    }
  }
  return core::Result::kSuccess;
}
}  // namespace hyped::debug
