#include "low_power_bmse.hpp"

namespace hyped::bms {

std::optional<LowPowerBMS> LowPowerBMS::create(core::ILogger &logger,
                                               std::shared_ptr<io::II2c> i2c,
                                               const std::uint8_t device_address)
{
  return LowPowerBMS(logger, i2c, device_address);
}

LowPowerBMS::LowPowerBMS(core::ILogger &logger, std::shared_ptr<io::II2c> i2c, const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c),
{
}

LowPowerBMS::~LowPowerBMS()
{
}

std::optional<core::BatteryStatus> LowPowerBMS::getBatteryData()
{
  const auto battery_status = i2c_->readByte(kBatteryStatusRegister);

  if (!battery_status) {
    logger_.log(core::LogLevel::kError, "Failed to read battery status from Low Power BMS");
    return std::nullopt;
  }

  core::BatteryStatus status;

  return status;
}

std:optional<core::BatteryStatus> LowPowerBMS::getCellData() {
  const auto battery_status = i2c_->readByte()
}

}  // namespace hyped::sensors
