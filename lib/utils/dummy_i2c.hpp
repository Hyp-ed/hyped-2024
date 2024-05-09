#pragma once

#include "dummy_adc.hpp"

#include <io/i2c.hpp>

namespace hyped::utils {

/**
 * A basic implementation of II2c that does not require any hardware. The purpose of this
 * class is to
 *
 *  1. demonstrate how to implement the II2c interface, and
 *  2. allow for quick test implementations.
 */
class DummyI2c : public io::II2c {
 public:
  virtual std::optional<std::uint8_t> readByte(const std::uint8_t device_address,
                                               const std::uint8_t register_address);
  virtual core::Result writeByteToRegister(const std::uint8_t device_address,
                                           const std::uint8_t register_address,
                                           const std::uint8_t data);
  virtual core::Result writeByteToRegister(const std::uint8_t device_address,
                                           const std::uint16_t register_address,
                                           const std::uint8_t data);
  virtual core::Result writeByte(const std::uint8_t device_address, const std::uint8_t data);

  void setWriteByteResults(std::vector<core::Result> results);
  void setReadByteResults(std::vector<std::optional<std::uint8_t>> results);

 private:
  std::vector<core::Result> write_byte_results_;
  std::vector<std::optional<std::uint8_t>> read_byte_results_;
};

}  // namespace hyped::utils
