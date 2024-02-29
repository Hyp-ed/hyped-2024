#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

static constexpr std::uint8_t kDefaultLpBmsAddress = 0x10;
static constexpr std::uint8_t kNumCells            = 16;

class LowPowerBMS {
 public:
  static std::optional<LowPowerBMS> create(core::ILogger &logger,
                                           std::shared_ptr<io::II2c> i2c,
                                           const std::uint8_t device_address);
  ~LowPowerBMS();

  std::optional<std::uint8_t> getStackVoltage();
  std::optional<std::array<std::uint8_t, kNumCells>> getCellData();
  std::optional<bool> checkUndervoltage();

 private:
  LowPowerBMS(core::ILogger &logger,
              std::shared_ptr<io::II2c> i2c,
              const std::uint8_t device_address);
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t device_address_;

 private:
  static constexpr std::uint8_t kCellVoltageRegisters[] = {
    0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x30, 0x32};
  static constexpr std::uint8_t kStackVoltage = 0x34;
  static constexpr std::uint8_t kUnderVoltage = 0x02;
};
}  // namespace hyped::sensors
