#include "low_power_bms.hpp"

namespace hyped::sensors {

std::optional<LowPowerBMS> LowPowerBMS::create(core::ILogger &logger,
                                               std::shared_ptr<io::II2c> i2c,
                                               const std::uint8_t device_address)
{
  if (device_address != kDefaultLpBmsAddress) {
    logger.log(core::LogLevel::kFatal, "Invalid device address for BMS");
    return std::nullopt;
  }
  const auto test = i2c->readByte(device_address, stack_voltage);
  if (!test) {
    logger.log(core::LogLevel::kFatal, "Failed to read values from BMS");
    return std::nullopt;
  }
  return LowPowerBMS(logger, i2c, device_address);
}

LowPowerBMS::LowPowerBMS(core::ILogger &logger,
                         std::shared_ptr<io::II2c> i2c,
                         const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c),
      device_address_(device_address)
{
}

LowPowerBMS::~LowPowerBMS()
{
}

std::optional<std::uint8_t> LowPowerBMS::getCellData()
{
  const std::uint8_t size = sizeof(cell_voltages);
  std::optional<std::uint8_t> voltages[size];
  for (int i = 0; i < size; i++) {
    voltages[i] = i2c_->readByte(device_address_, cell_voltages[i]);
  }
  return *voltages;
}

std::optional<std::uint8_t> LowPowerBMS::getStackVoltage()
{
  const auto voltage = i2c_->readByte(device_address_, stack_voltage);
  return voltage;
}
}  // namespace hyped::sensors
