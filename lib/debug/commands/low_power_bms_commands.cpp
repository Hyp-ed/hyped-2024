#include "low_power_bms_commands.hpp"

#include <cstdint>

namespace hyped::debug {
core::Result LowPowerBMSCommands::addCommands(core::ILogger &logger,
                                              std::shared_ptr<Repl> repl,
                                              toml::v3::node_view<toml::v3::node> config)
{
  // get bus
  const auto optional_bus = config["bus"].value<std::uint8_t>();
  if (!optional_bus) {
    logger.log(core::LogLevel::kFatal, "No I2C bus specified");
    return core::Result::kFailure;
  };
  const auto bus = *optional_bus;

  // get I2C instance
  const auto optional_i2c = repl->getI2c(bus);
  if (!optional_i2c) {
    logger.log(core::LogLevel::kFatal, "Error creating I2C bus");
    return core::Result::kFailure;
  };
  const auto i2c = std::move(*optional_i2c);

  // get address
  const auto optional_address = config["address"].value<std::uint8_t>();
  if (!optional_address) {
    logger.log(core::LogLevel::kFatal, "Invalid address");
    return core::Result::kFailure;
  }
  const auto address = std::move(*optional_address);

  // create sensor
  const auto optional_low_power_bms_sensor
    = sensors::LowPowerBMS::create(logger, i2c, sensors::kDefaultLpBmsAddress);
  if (!optional_low_power_bms_sensor) {
    logger.log(core::LogLevel::kFatal, "Failed to create low power BMS sensor");
    return core::Result::kFailure;
  }
  auto low_power_bms_sensor           = std::move(*optional_low_power_bms_sensor);
  const auto read_command_name        = "low power bms read";
  const auto read_command_description = "Read from the low power bms";
  const auto read_command_usage       = "low power bms read";
  const auto read_command_handler
    = [&logger, &low_power_bms_sensor](const std::vector<std::string> &) {
        // const std::array<std::uint8_t, sensors::kNumCells> cell_data[] =
        // low_power_bms_sensor.getCellData();
        const auto cell_data = low_power_bms_sensor.getCellData();
        if (!cell_data) {
          logger.log(core::LogLevel::kFatal, "Failed to get cell data");
          return;
        }
        // for (auto i = 0; i < sensors::kNumCells; i++){
        // 	logger.log(core::LogLevel::kDebug, "Cell data at %d is %u ", i, cell_data[i]);
        // }
        logger.log(core::LogLevel::kDebug, "Cell data: %u", cell_data);
        const auto stack_voltage = low_power_bms_sensor.getStackVoltage();
        if (!stack_voltage) {
          logger.log(core::LogLevel::kFatal, "Failed to get stack voltage");
          return;
        }
        logger.log(core::LogLevel::kDebug, "Stack voltage: %u V", stack_voltage);
        const auto undervoltage = low_power_bms_sensor.checkUndervoltage();
        if (!undervoltage) {
          logger.log(core::LogLevel::kFatal, "Failed to check for undervoltage");
        }
        logger.log(core::LogLevel::kDebug, "Undervoltage: %s", undervoltage ? "true" : "false");
      };
  auto read_command = std::make_unique<Command>(
    read_command_name, read_command_description, read_command_usage, read_command_handler);
  repl->addCommand(std::move(read_command));
  return core::Result::kSuccess;
};
}  // namespace hyped::debug
