#include "dummy_i2c.hpp"

#include <utility>

namespace hyped::utils {

std::optional<std::uint8_t> DummyI2c::readByte(const std::uint8_t device_address,
                                               const std::uint8_t register_address)
{
  if (read_byte_results_.empty()) { return std::nullopt; }
  const auto next_byte = read_byte_results_.at(0);
  read_byte_results_.erase(read_byte_results_.begin());
  return next_byte;
}

core::Result DummyI2c::writeByteToRegister(const std::uint8_t device_address,
                                           const std::uint8_t register_address,
                                           const std::uint8_t data)
{
  return writeByte(0, 0);
}

core::Result DummyI2c::writeByteToRegister(const std::uint8_t device_address,
                                           const std::uint16_t register_address,
                                           const std::uint8_t data)
{
  return writeByte(0, 0);
}

core::Result DummyI2c::writeByte(const std::uint8_t device_address, const std::uint8_t data)
{
  if (write_byte_results_.empty()) { return core::Result::kFailure; }
  const auto next_result = write_byte_results_.at(0);
  write_byte_results_.erase(write_byte_results_.begin());
  return next_result;
}

void DummyI2c::setWriteByteResults(std::vector<core::Result> results)
{
  write_byte_results_ = std::move(results);
}

void DummyI2c::setReadByteResults(std::vector<std::optional<std::uint8_t>> results)
{
  read_byte_results_ = std::move(results);
}

}  // namespace hyped::utils
