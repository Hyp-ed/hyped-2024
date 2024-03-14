#include "low_power_bms.hpp"

namespace hyped::sensors {

std::optional<LowPowerBMS> LowPowerBMS::create(core::ILogger &logger,
                                               std::shared_ptr<io::II2c> i2c,
                                               const std::uint8_t device_address)
{
  const auto test = i2c->readByte(device_address, kStackVoltage);
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

std::optional<std::array<std::uint8_t, kNumCells>> LowPowerBMS::getCellData()
{
  std::array<std::uint8_t, kNumCells> voltages;
  for (auto i = 0; i < kNumCells; i++) {
    const auto voltage = i2c_->readByte(device_address_, kCellVoltageRegisters[i]);
    if (!voltage) { return std::nullopt; }
    voltages[i] = *voltage;
  }
  return voltages;
}

std::optional<std::uint8_t> LowPowerBMS::getStackVoltage()
{
  const auto voltage = i2c_->readByte(device_address_, kStackVoltage);
  if (!voltage) { return std::nullopt; }
  return voltage;
}

std::optional<bool> LowPowerBMS::checkUndervoltage()
{
  const auto status = i2c_->readByte(device_address_, kUnderVoltage);
  if (!status) { return std::nullopt; }
  if (*status & 0b00000100) { return true; }
  return false;
}

}  // namespace hyped::sensors
