#include "low_power_bms.hpp"

namespace hyped::sensors {

std::optional<LowPowerBMS> LowPowerBMS::create(core::ILogger &logger,
                                               std::shared_ptr<io::II2c> i2c,
                                               const std::uint8_t device_address)
{
  if (device_address != kDefaultLpBmsReadAddress) {
    logger.log(core::LogLevel::kFatal, "Invalid device address for accelerometer");
    return std::nullopt;
  }
  return LowPowerBMS(logger, i2c, device_address);
}

LowPowerBMS::LowPowerBMS(core::ILogger &logger,
                         std::shared_ptr<io::II2c> i2c,
                         const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c)
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
    voltages[i] = i2c_->readByte(kDefaultLpBmsReadAddress, cell_voltages[i]);
  }
  return *voltages;
}

std::optional<std::uint8_t> LowPowerBMS::getStackVoltage()
{
  const auto voltage = i2c_->readByte(kDefaultLpBmsReadAddress, stack_voltage);
  return voltage;
}
}  // namespace hyped::sensors
