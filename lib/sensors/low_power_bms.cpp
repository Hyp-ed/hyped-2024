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

LowPowerBMS::LowPowerBMS(core::ILogger &logger, std::shared_ptr<io::II2c> i2c, const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c)
{
}

LowPowerBMS::~LowPowerBMS()
{
}

const u_int32_t LowPowerBMS::getCellData()
{
  const uint8_t size = sizeof(cell_voltages);
  const u_int32_t voltages[size] = {};
  for (int i = 0; i < size; i++) {
    voltages[i] = i2c_->readByte(kDefaultLpBmsReadAddress, cell_voltages[i]);
  }
  return voltages;
}

const u_int32_t LowPowerBMS::getStackVoltage() {
  const u_int32_t stack_voltage = i2c_->readByte(kDefaultLpBmsReadAddress, stack_voltage);
  return stack_voltage;
}
}  // namespace hyped::sensors
