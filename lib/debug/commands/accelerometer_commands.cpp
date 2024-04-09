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

  // instance of sensor
  const auto accelerometer_sensor           = std::make_shared<sensors::Accelerometer>(logger, i2c);
  const auto accelerometer_read             = "read";
  const auto accelerometer_read_description = "Read accelerometer";
  const auto accelerometer_read_handler     = [&logger, accelerometer_sensor]() {
    const auto value_ready  = accelerometer_sensor->isValueReady();  // method already returns state
    const auto acceleration = accelerometer_sensor->read();
    if (!acceleration) {
      logger.log(core::LogLevel::kFatal, "Failed to read acceleration");
      return;
    }
    logger.log(core::LogLevel::kDebug, "Acceleration: %d", *acceleration);
  };
  auto read_command = std::make_unique<Command>(
    accelerometer_read, accelerometer_read_description, accelerometer_read_handler);
  repl->addCommand(std::move(accelerometer_read));
  return core::Result::kSuccess;
}
}  // namespace hyped::debug