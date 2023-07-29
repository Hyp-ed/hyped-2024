#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

// Four possible device addresses, as per the datasheet
// 0x40 has been chosen as the default
constexpr std::uint8_t kDefaultLowPowerCurrentAddress = 0x40;
constexpr std::uint8_t kLowPowerCurrentAddress2       = 0x41;
constexpr std::uint8_t kLowPowerCurrentAddress3       = 0x44;
constexpr std::uint8_t kLowPowerCurrentAddress4       = 0x45;

// TODOLater: Test this code with hardware
class LowPowerCurrent {
 public:
  static std::optional<LowPowerCurrent> create(core::ILogger &logger,
                                               std::shared_ptr<io::II2c> i2c,
                                               const std::uint8_t device_address);
  ~LowPowerCurrent();

  std::optional<core::Float> readCurrent();

 private:
  LowPowerCurrent(core::ILogger &logger,
                  std::shared_ptr<io::II2c> i2c,
                  const std::uint8_t device_address);

 private:
  // Registers with reference to the INA219 driver code
  static const std::uint8_t kLowPowerCurrentRegister = 0x04;

  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t device_address_;
};

}  // namespace hyped::sensors