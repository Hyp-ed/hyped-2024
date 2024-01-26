#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

static constexpr std::uint8_t kDefaultLedDriverAddress = 0x10;

namespace hyped::bms {

class LowPowerBMS {
 public:
  static std::optional<LowPowerBMS> create(core::ILogger &logger,
                                           std::shared_ptr<io::II2c> i2c,
                                           const std::uint8_t device_address);
  ~LowPowerBMS();

  std::optional<core::BatteryStatus> getBatteryStatus();

 private:
  LowPowerBMS(core::ILogger &logger, std::shared_ptr<io::II2c> i2c, const std::uint8_t device_address);

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t device_address_;

  static constexpr std::uint8_t kBatteryStatusRegister = 0x10;

};

}  // namespace hyped::bms
