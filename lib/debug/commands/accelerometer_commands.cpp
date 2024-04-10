#include "accelerometer_commands.hpp"

#include <cstdint>

namespace hyped::debug {
core::Result AccelerometerCommands::addCommands(core::ILogger &logger,
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
  const auto optional_accelerometer_sensor = sensors::Accelerometer::create(
    logger, i2c, 2, static_cast<sensors::accelerometerAddress>(address));
  if (!optional_accelerometer_sensor) {
    logger.log(core::LogLevel::kFatal, "Failed to create accelerometer sensor");
    return core::Result::kFailure;
  }
  auto accelerometer_sensor           = std::move(*optional_accelerometer_sensor);
  const auto read_command_name        = "accelerometer read";
  const auto read_command_description = "Read from the accelerometer";
  const auto read_command_handler     = [&logger, &accelerometer_sensor]() {
    const auto value_ready = accelerometer_sensor.isValueReady();
    if (!value_ready) {
      logger.log(core::LogLevel::kFatal, "Value is not ready");
      return;
    }
    const auto acceleration = accelerometer_sensor.read();
    if (!acceleration) {
      logger.log(core::LogLevel::kFatal, "Failed to read acceleration");
      return;
    }
    logger.log(core::LogLevel::kDebug, "Acceleration: %s m/s", acceleration);
  };
  auto read_command
    = std::make_unique<Command>(read_command_name, read_command_description, read_command_handler);
  repl->addCommand(std::move(read_command));
  return core::Result::kSuccess;
}

}  // namespace hyped::debug