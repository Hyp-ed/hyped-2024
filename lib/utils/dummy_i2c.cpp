#include "dummy_i2c.hpp"

namespace hyped::utils {

DummyI2c::DummyI2c()
{
}

std::optional<std::uint8_t> DummyI2c::readByte(const std::uint8_t device_address,
                                               const std::uint8_t register_address)
{
  return std::nullopt;
}

core::Result DummyI2c::writeByteToRegister(const std::uint8_t device_address,
                                           const std::uint8_t register_address,
                                           const std::uint8_t data)
{
  return core::Result::kFailure;
}

core::Result DummyI2c::writeByte(const std::uint8_t device_address, const std::uint8_t data)
{
  return core::Result::kFailure;
}

}  // namespace hyped::utils
