#pragma once

#include "i2c.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include <core/logger.hpp>
#include <core/types.hpp>

namespace hyped::io {

class NucleoI2c : public II2c {
 public:
 static std::optional<std::shared_ptr<NucleoI2c>> create(core::ILogger &logger,
 														 const std::uint8_t bus);
 
 NucleoI2c(core::ILogger &logger, const int file_descriptor);
  ~NucleoI2c();

  std::optional<std::uint8_t> readByte(const std::uint8_t device_address,
                                       const std::uint8_t register_address);
  core::Result writeByteToRegister(const std::uint8_t device_address,
                                   const std::uint8_t register_address,
                                   const std::uint8_t data);
  core::Result writeByteToRegister(const std::uint8_t device_address,
                                   const std::uint16_t register_address,
                                   const std::uint8_t data);
  core::Result writeByte(const std::uint8_t device_address, const std::uint8_t data);

 private:
  void setSensorAddress(const std::uint8_t device_address);
  core::Result writeBytesToDevice(const std::uint8_t device_address,
                                  const std::vector<std::uint8_t> &bytes);

 private:
  core::ILogger &logger_;
  std::uint8_t sensor_address_;
  const int file_descriptor_;

};

} //namespace hyped::io