#pragma once

#include <optional>

#include <core/types.hpp>

namespace hyped::io {

class II2c {
 public:
  /**
   * @brief      Reads a byte from some device on the I2C bus.
   */
  virtual std::optional<std::uint8_t> readByte(const std::uint8_t device_address,
                                               const std::uint8_t register_address)
    = 0;

  /**
   * @brief      General function to write a byte to a register to some device on the I2C bus
   */
  virtual core::Result writeByteToRegister(const std::uint8_t device_address,
                                           const std::uint8_t register_address,
                                           const std::uint8_t data)
    = 0;

  /**
   * @brief      Writes a byte to single register devices such as the mux
   */
  virtual core::Result writeByte(const std::uint8_t device_address, std::uint8_t data) = 0;
};

}  // namespace hyped::io
