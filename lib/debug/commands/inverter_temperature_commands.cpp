#include "inverter_temperature_commands.hpp"

#include <cstdint>

namespace hyped::debug {

core::Result InverterTemperatureCommands::addCommands(core::ILogger &logger,
                                                      std::shared_ptr<Repl> repl,
                                                      toml::v3::node_view<toml::v3::node> config)
{
  // Get the I2C bus number from the config file
  const auto optional_bus = config["bus"].value<std::uint8_t>();
  if (!optional_bus) {
    logger.log(core::LogLevel::kFatal, "No I2C bus specified");
    return core::Result::kFailure;
  };
  const auto bus = *optional_bus;
  // Get the ADC mux channel from the config file
  const auto optional_adc_mux_channel = config["adc_mux_channel"].value<std::uint8_t>();
  if (!optional_adc_mux_channel) {
    logger.log(core::LogLevel::kFatal, "No ADC mux channel specified");
    return core::Result::kFailure;
  };
  const auto adc_mux_channel = *optional_adc_mux_channel;
  // Get the I2C instance
  const auto optional_i2c = repl->getI2c(bus);
  if (!optional_i2c) {
    logger.log(core::LogLevel::kFatal, "Error getting I2C bus");
    return core::Result::kFailure;
  };
  const auto i2c = std::move(*optional_i2c);

  // Create the sensor instance
  auto inverter_temperature_sensor = std::make_shared<sensors::InverterTemperature>(
    logger, i2c, static_cast<sensors::AdcMuxChannel>(adc_mux_channel));

  // Create the read command
  const auto read_command_name        = "temperature read";
  const auto read_command_description = "Read inverter temperature";
  const auto read_command_handler     = [&logger, inverter_temperature_sensor]() {
    const auto temperature = inverter_temperature_sensor->readTemperature();
    if (!temperature) {
      logger.log(core::LogLevel::kFatal, "Failed to read inverter temperature");
      return;
    }
    logger.log(core::LogLevel::kDebug, "Inverter temperature: %d", *temperature);
  };
  auto read_command
    = std::make_unique<Command>(read_command_name, read_command_description, read_command_handler);
  repl->addCommand(std::move(read_command));
  return core::Result::kSuccess;
}

}  // namespace hyped::debug