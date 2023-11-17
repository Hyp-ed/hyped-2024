#include "spi_commands.hpp"

namespace hyped::debug {

core::Result SpiCommands::addCommands(core::ILogger &logger,
                                      std::shared_ptr<Repl> repl,
                                      toml::v3::node_view<toml::v3::node> config)
{
  const auto buses = config["buses"].as_array();
  if (!buses) {
    logger.log(core::LogLevel::kFatal, "No SPI buses specified");
    return core::Result::kFailure;
  }
  for (const auto &spi_bus : *buses) {
    const auto optional_bus = spi_bus.value<std::uint8_t>();
    if (!optional_bus) {
      logger.log(core::LogLevel::kFatal, "Invalid SPI bus");
      return core::Result::kFailure;
    }
    const auto bus          = *optional_bus;
    const auto optional_spi = repl->getSpi(static_cast<io::SpiBus>(bus),
                                           io::SpiMode::kMode3,
                                           io::SpiWordSize::kWordSize8,
                                           io::SpiBitOrder::kMsbFirst,
                                           io::SpiClock::k500KHz);
    if (!optional_spi) {
      logger.log(core::LogLevel::kFatal, "Failed to create SPI instance on bus %d", bus);
      return core::Result::kFailure;
    }
    const auto spi = std::move(*optional_spi);
    {
      const auto spi_read_byte_command_name = "spi " + std::to_string(bus) + " read byte";
      const auto spi_read_byte_command_description
        = "Read a byte from SPI bus " + std::to_string(bus);
      const auto spi_read_byte_command_handler = [&logger, spi, bus] {
        std::uint16_t register_address;
        std::cout << "Register address: ";
        std::cin >> register_address;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                                                             spi_read_byte_command_handler);
      repl->addCommand(std::move(spi_read_byte_command));
    }
    {
      const auto spi_write_byte_command_name = "spi " + std::to_string(bus) + " write byte";
      const auto spi_write_byte_command_description
        = "Write a byte to SPI bus " + std::to_string(bus);
      const auto spi_write_byte_command_handler = [&logger, spi, bus] {
        std::uint32_t register_address, data;
        std::cout << "Register address: ";
        std::cin >> std::hex >> register_address;
        std::cout << "Data: ";
        std::cin >> std::hex >> data;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                                                              spi_write_byte_command_handler);
      repl->addCommand(std::move(spi_write_byte_command));
    }
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::debug