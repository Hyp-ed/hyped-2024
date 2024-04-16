#include "i2c_commands.hpp"

namespace hyped::debug {

core::Result I2cCommands::addCommands(core::ILogger &logger,
                                      std::shared_ptr<Repl> repl,
                                      toml::v3::node_view<toml::v3::node> config)
{
  {
    const auto read_command_name        = "i2c read";
    const auto read_command_description = "Read from an I2C bus";
    const auto read_command_usage       = "i2c read <bus> <device_address> <register_address>";
    const auto read_command_handler     = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 3) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto bus          = std::stoi(args[0]);
      const auto optional_i2c = repl->getI2c(bus);
      if (!optional_i2c) {
        logger.log(core::LogLevel::kFatal, "Error getting I2C bus");
        return;
      };
      const auto i2c                       = std::move(*optional_i2c);
      const std::uint32_t device_address   = std::stoul(args[1], nullptr, 16);
      const std::uint32_t register_address = std::stoul(args[2], nullptr, 16);
      const auto value                     = i2c->readByte(device_address, register_address);
      if (!value) {
        logger.log(core::LogLevel::kFatal, "Failed to read from I2C bus %i", bus);
        return;
      }
      logger.log(core::LogLevel::kDebug, "I2C value from bus %i: %d", bus, *value);
    };
    auto read_command = std::make_unique<Command>(
      read_command_name, read_command_description, read_command_usage, read_command_handler);
    repl->addCommand(std::move(read_command));
  }
  {
    const auto write_command_name        = "i2c write";
    const auto write_command_description = "Write to an I2C bus";
    const auto write_command_usage   = "i2c write <bus> <device_address> <register_address> <data>";
    const auto write_command_handler = [&logger, repl](const std::vector<std::string> args) {
      if (args.size() != 4) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto bus          = std::stoi(args[0]);
      const auto optional_i2c = repl->getI2c(bus);
      if (!optional_i2c) {
        logger.log(core::LogLevel::kFatal, "Error getting I2C bus");
        return;
      };
      const auto i2c = std::move(*optional_i2c);
      // TODO stop using std::stoul
      const std::uint8_t device_address   = std::stoul(args[1], nullptr, 16);
      const std::uint8_t register_address = std::stoul(args[2], nullptr, 16);
      const std::uint32_t data            = std::stoul(args[3], nullptr, 16);
      const core::Result result = i2c->writeByteToRegister(device_address, register_address, data);
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to write to I2C bus: %d", bus);
        return;
      }
      logger.log(
        core::LogLevel::kDebug, "I2C write successful to device %d on %d", device_address, bus);
    };
    auto write_command = std::make_unique<Command>(
      write_command_name, write_command_description, write_command_usage, write_command_handler);
    repl->addCommand(std::move(write_command));
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::debug
