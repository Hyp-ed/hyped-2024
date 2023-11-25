#include "i2c_commands.hpp"

namespace hyped::debug {

core::Result I2cCommands::addCommands(core::ILogger &logger,
                                      std::shared_ptr<Repl> repl,
                                      toml::v3::node_view<toml::v3::node> config)
{
  const auto buses = config["buses"].as_array();
  if (!buses) {
    logger.log(core::LogLevel::kFatal, "No I2C buses specified");
    return core::Result::kFailure;
  }
  for (const auto &i2c_bus : *buses) {
    const auto optional_bus = i2c_bus.value<std::uint8_t>();
    if (!optional_bus) {
      logger.log(core::LogLevel::kFatal, "Invalid I2C bus name");
      return core::Result::kFailure;
    }
    const auto bus          = *optional_bus;
    const auto optional_i2c = repl->getI2c(bus);
    if (!optional_i2c) {
      logger.log(core::LogLevel::kFatal, "Error creating I2C bus");
      return core::Result::kFailure;
    };
    const auto i2c                      = std::move(*optional_i2c);
    const auto read_command_name        = std::to_string(bus) + " read";
    const auto read_command_description = "Read from I2C bus " + std::to_string(bus);
    const auto read_command_handler     = [&logger, i2c, bus]() {
      std::cout << "Enter I2C device address: ";
      std::uint32_t device_address;
      std::cin >> std::hex >> device_address;
      std::cout << "Enter I2C register address: ";
      std::uint32_t register_address;
      std::cin >> std::hex >> register_address;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      const auto value = i2c->readByte(device_address, register_address);
      if (!value) {
        logger.log(core::LogLevel::kFatal, "Failed to read from I2C bus %i", bus);
        return;
      }
      logger.log(core::LogLevel::kDebug, "I2C value from bus %i: %d", bus, *value);
    };
    auto read_command = std::make_unique<Command>(
      read_command_name, read_command_description, read_command_handler);
    repl->addCommand(std::move(read_command));
    const auto write_command_name        = std::to_string(bus) + " write";
    const auto write_command_description = "Write to I2C bus " + std::to_string(bus);
    const auto write_command_handler     = [&logger, i2c, bus]() {
      std::uint32_t device_address, register_address, data;
      std::cout << "Device address: ";
      std::cin >> std::hex >> device_address;
      std::cout << "Register address: ";
      std::cin >> std::hex >> register_address;
      std::cout << "Data: ";
      std::cin >> std::hex >> data;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      const core::Result result = i2c->writeByteToRegister(device_address, register_address, data);
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to write to I2C bus: %d", bus);
        return;
      }
      logger.log(
        core::LogLevel::kDebug, "I2C write successful to device %d on %d", device_address, bus);
    };
    auto write_command = std::make_unique<Command>(
      write_command_name, write_command_description, write_command_handler);
    repl->addCommand(std::move(write_command));
  }
  return core::Result::kSuccess;
};

}  // namespace hyped::debug
