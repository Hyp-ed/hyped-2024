#include "temperature_commands.hpp"

#include <sensors/temperature.hpp>

namespace hyped::debug {

core::Result TemperatureCommands::addCommands(core::ILogger &logger,
                                              std::shared_ptr<Repl> repl,
                                              toml::v3::node_view<toml::v3::node> config)
{
  const auto optional_bus = config["bus"].value<std::uint8_t>();
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
  const auto i2c              = std::move(*optional_i2c);
  const auto optional_address = config["address"].value<std::uint8_t>();
  if (!optional_address) {
    logger.log(core::LogLevel::kFatal, "Invalid address");
    return core::Result::kFailure;
  }
  const auto address                     = std::move(*optional_address);
  const auto optional_temperature_sensor = sensors::Temperature::create(
    logger, i2c, 0, static_cast<sensors::temperatureAddress>(address));
  if (!optional_temperature_sensor) {
    logger.log(core::LogLevel::kFatal, "Failed to create temperature sensor");
    return core::Result::kFailure;
  }
  auto temperature_sensor             = std::move(*optional_temperature_sensor);
  const auto read_command_name        = "temperature read";
  const auto read_command_description = "Read from the temperature sensor";
  const auto read_command_handler     = [&logger, &temperature_sensor]() {
    const auto value = temperature_sensor.read();
    if (!value) {
      logger.log(core::LogLevel::kFatal, "Failed to read from temperature sensor");
      return;
    }
    logger.log(core::LogLevel::kDebug, "Read temperature: %d °C", *value);
  };
  auto read_command
    = std::make_unique<Command>(read_command_name, read_command_description, read_command_handler);
  repl->addCommand(std::move(read_command));
  return core::Result::kSuccess;
}

}  // namespace hyped::debug
