#include <low_power_current.hpp>

namespace hyped::sensors {

// TODOLater: Confirm failure states for create
std::optional<LowPowerCurrent> LowPowerCurrent::create(core::ILogger &logger,
                                                       std::shared_ptr<io::II2c> i2c,
                                                       const std::uint8_t device_address)
{
  return LowPowerCurrent(logger, i2c, device_address);
}

LowPowerCurrent::LowPowerCurrent(core::ILogger &logger,
                                 std::shared_ptr<io::II2c> i2c,
                                 const std::uint8_t device_address)
    : logger_(logger),
      i2c_(i2c),
      device_address_(device_address)
{
}

LowPowerCurrent::~LowPowerCurrent()
{
}

std::optional<core::Float> LowPowerCurrent::readCurrent()
{
  const auto low_power_current = i2c_->readByte(device_address_, kLowPowerCurrentRegister);
  if (!low_power_current) {
    logger_.log(core::LogLevel::kFatal, "Failed to read low power current");
    return std::nullopt;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully read low power current");
  // Divide milliAmps output by 1,000 to return Amps value
  return static_cast<core::Float>(*low_power_current) / 1000;
}

}  // namespace hyped::sensors