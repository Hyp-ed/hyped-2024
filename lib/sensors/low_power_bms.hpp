#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

static constexpr std::uint8_t kDefaultLpBmsReadAddress = 0x10;
static constexpr std::uint8_t kDefaultLpBmsWriteAddress = 0x11;

namespace hyped::bms {

class LowPowerBMS {
 public:
  static std::optional<LowPowerBMS> create(core::ILogger &logger,
                                           std::shared_ptr<io::II2c> i2c,);
  ~LowPowerBMS();

  std::optional<core::BatteryStatus> getBatteryStatus();

 private:
  LowPowerBMS(core::ILogger &logger, std::shared_ptr<io::II2c> i2c);
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t device_address = kDefaultLpBmsAddress;

  static constexpr std::uint8_t kBatteryStatusRegister;

};

}  // namespace hyped::sensors
