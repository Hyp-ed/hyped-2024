#include "nucleo_i2c.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>

#if LINUX
#include <linux/i2c-dev.h>
#else
#define I2C_SLAVE 0x0703  // To specify that we are making I2C transactions
#endif

namespace hyped::io {

std::optional<std::shared_ptr<NucleoI2c>> NucleoI2c::create(core::ILogger &logger,
                                                                const std::uint8_t bus)
{
  // Up to 2 buses on STM boards
  if (bus > 2) {
    logger.log(core::LogLevel::kFatal, "Failed to create NucleoI2c object: invalid bus");
    return std::nullopt;
  }
  char path[13];  // up to "/dev/i2c-2"
  snprintf(path, sizeof(path), "/dev/i2c-%d", bus);
  const int file_descriptor = open(path, O_RDWR, 0);
  if (file_descriptor < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to find i2c device");
    return std::nullopt;
  };
  return std::make_shared<NucleoI2c>(logger, file_descriptor);
}

NucleoI2c::NucleoI2c(core::ILogger &logger, const int file_descriptor)
    : logger_(logger),
      file_descriptor_(file_descriptor),
      sensor_address_(0)
{
}

NucleoI2c::~NucleoI2c()
{
  close(file_descriptor_);
}

std::optional<std::uint8_t> NucleoI2c::readByte(const std::uint8_t device_address,
                                                  const std::uint8_t register_address)
{
  if (sensor_address_ != device_address) { setSensorAddress(device_address); }
  // Contains data which we will read
  std::uint8_t read_buffer[1];
  // Contains data which we need to write
  const std::uint8_t write_buffer[1] = {register_address};
  // Writing the register address so we switch to it
  const int num_bytes_written = write(file_descriptor_, write_buffer, 1);
  if (num_bytes_written != 1) {
    logger_.log(core::LogLevel::kFatal, "Failed to write to i2c device");
    return std::nullopt;
  }
  const int num_bytes_read = read(file_descriptor_, read_buffer, 1);
  if (num_bytes_read != 1) {
    logger_.log(core::LogLevel::kFatal, "Failed to read from i2c device");
    return std::nullopt;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully read byte from i2c device");
  return read_buffer[0];
}

core::Result NucleoI2c::writeByteToRegister(const std::uint8_t device_address,
                                              const std::uint8_t register_address,
                                              const std::uint8_t data)
{
  const std::vector<std::uint8_t> bytes = {register_address, data};
  return writeBytesToDevice(device_address, bytes);
}

core::Result NucleoI2c::writeByteToRegister(const std::uint8_t device_address,
                                              const std::uint16_t register_address,
                                              const std::uint8_t data)
{
  const std::uint8_t register_address_hi = register_address >> 8;
  const std::uint8_t register_address_lo = static_cast<std::uint8_t>(register_address);
  const std::vector<std::uint8_t> bytes  = {register_address_hi, register_address_lo, data};
  return writeBytesToDevice(device_address, bytes);
}

// TODOLater - Test code
core::Result NucleoI2c::writeBytesToDevice(const std::uint8_t device_address,
                                             const std::vector<std::uint8_t> &bytes)
{
  if (sensor_address_ != device_address) { setSensorAddress(device_address); }
  const std::size_t number_of_bytes = bytes.size();
  const std::uint8_t *write_buffer  = bytes.data();
  const ssize_t num_bytes_written   = write(file_descriptor_, write_buffer, number_of_bytes);
  if (num_bytes_written != number_of_bytes) {
    logger_.log(core::LogLevel::kFatal, "Failed to write to i2c device");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully wrote bytes to i2c device register");
  return core::Result::kSuccess;
}

core::Result NucleoI2c::writeByte(const std::uint8_t device_address, const std::uint8_t data)
{
  if (sensor_address_ != device_address) { setSensorAddress(device_address); }
  const std::uint8_t write_buffer[1] = {data};
  const ssize_t num_bytes_written    = write(file_descriptor_, write_buffer, 1);
  if (num_bytes_written != 1) {
    logger_.log(core::LogLevel::kFatal, "Failed to write to i2c device");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully wrote byte to i2c device");
  return core::Result::kSuccess;
}

void NucleoI2c::setSensorAddress(const std::uint8_t device_address)
{
  const int return_value = ioctl(file_descriptor_, I2C_SLAVE, device_address);
  if (return_value < 0) {
    logger_.log(core::LogLevel::kFatal, "Failed to set sensor address");
    return;
  }
  sensor_address_ = device_address;
}

}  // namespace hyped::io