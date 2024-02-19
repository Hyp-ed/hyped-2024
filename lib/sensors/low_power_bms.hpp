#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

static constexpr std::uint8_t kDefaultLpBmsReadAddress  = 0x10;
static constexpr std::uint8_t kDefaultLpBmsWriteAddress = 0x11;

namespace hyped::sensors {

class LowPowerBMS {
 public:
  static std::optional<LowPowerBMS> create(core::ILogger &logger,
                                           std::shared_ptr<io::II2c> i2c,
                                           const std::uint8_t device_address);
  ~LowPowerBMS();

  std::optional<std::uint8_t> getStackVoltage();
  std::optional<std::uint8_t> getCellData();

 private:
  LowPowerBMS(core::ILogger &logger,
              std::shared_ptr<io::II2c> i2c,
              const std::uint8_t device_address);
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;

  const std::uint8_t device_read_address  = kDefaultLpBmsReadAddress;
  const std::uint8_t device_write_address = kDefaultLpBmsWriteAddress;

 private:
  static constexpr std::uint8_t cell_voltages[] = {
    0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x30, 0x32};
  static constexpr std::uint8_t stack_voltage = 0x34;
};
}  // namespace hyped::sensors
