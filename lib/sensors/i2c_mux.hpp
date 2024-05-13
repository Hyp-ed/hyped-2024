#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

constexpr std::uint8_t kDefaultMuxAddress = 0x70;
constexpr std::uint8_t kMaxMuxChannel     = 8;

/**
 * @brief Mux for sensors using I2C. Construct this and pass it to the sensor in place of II2c.
 */
class I2cMux : public io::II2c {
 public:
  static std::optional<std::shared_ptr<I2cMux>> create(core::ILogger &logger,
                                                       std::shared_ptr<io::II2c> i2c,
                                                       const std::uint8_t mux_address,
                                                       const std::uint8_t channel);
  I2cMux(core::ILogger &logger,
         std::shared_ptr<io::II2c> i2c,
         const std::uint8_t mux_address,
         const std::uint8_t channel);

  std::optional<std::uint8_t> readByte(const std::uint8_t device_address,
                                       const std::uint8_t register_address) override;
  core::Result writeByte(const std::uint8_t device_address, const std::uint8_t data) override;
  core::Result writeByteToRegister(const std::uint8_t device_address,
                                   const std::uint8_t register_address,
                                   const std::uint8_t data) override;
  core::Result writeByteToRegister(const std::uint8_t device_address,
                                   const std::uint16_t register_address,
                                   const std::uint8_t data) override;

 private:
  core::Result selectChannel();

  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t mux_address_;
  const std::uint8_t channel_;
};

}  // namespace hyped::sensors
