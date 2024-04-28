#include "spi_commands.hpp"

namespace hyped::debug {

core::Result SpiCommands::addCommands(core::ILogger &logger, std::shared_ptr<Repl> repl)
{
  {
    const auto spi_read_byte_command_name        = "spi read byte";
    const auto spi_read_byte_command_description = "Read a byte from an SPI bus";
    const auto spi_read_byte_command_usage
      = "spi read byte <bus> <mode> <word_size> <bit_order> <clock> <register_address>";
    const auto spi_read_byte_command_handler = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 6) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto bus       = static_cast<io::SpiBus>(std::stoi(args[0]));
      const auto mode      = static_cast<io::SpiMode>(std::stoi(args[1]));
      const auto word_size = static_cast<io::SpiWordSize>(std::stoi(args[2]));
      const auto bit_order = static_cast<io::SpiBitOrder>(std::stoi(args[3]));
      const auto clock     = static_cast<io::SpiClock>(std::stoi(args[4]));

      const auto optional_spi = repl->getSpi(bus, mode, word_size, bit_order, clock);

      if (!optional_spi) {
        logger.log(core::LogLevel::kFatal, "Failed to get SPI instance on bus %d", bus);
        return;
      }
      const auto spi = std::move(*optional_spi);

      std::uint16_t register_address = std::stoi(args[5], nullptr, 16);
      std::uint8_t read_buffer;
      const core::Result result = spi->read(register_address, &read_buffer, 1);
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to read from SPI bus %d", bus);
        return;
      }
      logger.log(core::LogLevel::kDebug, "SPI value from bus %d: %d", bus, read_buffer);
    };
    auto spi_read_byte_command = std::make_unique<Command>(spi_read_byte_command_name,
                                                           spi_read_byte_command_description,
                                                           spi_read_byte_command_usage,
                                                           spi_read_byte_command_handler);
    repl->addCommand(std::move(spi_read_byte_command));
  }
  {
    const auto spi_write_byte_command_name        = "spi write byte";
    const auto spi_write_byte_command_description = "Write a byte to an SPI bus";
    const auto spi_write_byte_command_usage
      = "spi write byte <bus> <mode> <word_size> <bit_order> <clock> <register_address> <data>";
    const auto spi_write_byte_command_handler = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 7) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto bus       = static_cast<io::SpiBus>(std::stoi(args[0]));
      const auto mode      = static_cast<io::SpiMode>(std::stoi(args[1]));
      const auto word_size = static_cast<io::SpiWordSize>(std::stoi(args[2]));
      const auto bit_order = static_cast<io::SpiBitOrder>(std::stoi(args[3]));
      const auto clock     = static_cast<io::SpiClock>(std::stoi(args[4]));

      const auto optional_spi = repl->getSpi(bus, mode, word_size, bit_order, clock);
      if (!optional_spi) {
        logger.log(core::LogLevel::kFatal, "Failed to get SPI instance on bus %d", bus);
        return;
      }
      const auto spi = std::move(*optional_spi);

      std::uint16_t register_address = std::stoi(args[5], nullptr, 16);
      std::uint8_t data              = std::stoi(args[6], nullptr, 16);

      const std::uint8_t *data_ptr = reinterpret_cast<const std::uint8_t *>(&data);
      const core::Result result    = spi->write(register_address, data_ptr, 1);
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to write to SPI bus: %d", bus);
        return;
      }
      logger.log(
        core::LogLevel::kDebug, "Successful SPI write to device %d on %d", register_address, bus);
    };
    auto spi_write_byte_command = std::make_unique<Command>(spi_write_byte_command_name,
                                                            spi_write_byte_command_description,
                                                            spi_write_byte_command_usage,
                                                            spi_write_byte_command_handler);
    repl->addCommand(std::move(spi_write_byte_command));
  }

  return core::Result::kSuccess;
}

}  // namespace hyped::debug