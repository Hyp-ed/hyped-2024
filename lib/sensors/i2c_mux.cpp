#include "i2c_mux.hpp"

#include <memory>
#include <utility>

#include "core/types.hpp"

namespace hyped::sensors {

std::optional<std::shared_ptr<I2cMux>> I2cMux::create(core::ILogger &logger,
                                                      std::shared_ptr<io::II2c> i2c,
                                                      const std::uint8_t mux_address,
                                                      const std::uint8_t channel)
{
  if (channel >= kMaxMuxChannel) {
    logger.log(core::LogLevel::kFatal, "I2c Mux Channel number %d is not selectable", channel);
    return std::nullopt;
  }
  return std::make_shared<I2cMux>(logger, std::move(i2c), mux_address, channel);
}

I2cMux::I2cMux(core::ILogger &logger,
               std::shared_ptr<io::II2c> i2c,
               const std::uint8_t mux_address,
               const std::uint8_t channel)
    : logger_(logger),
      i2c_(std::move(i2c)),
      mux_address_(mux_address),
      channel_(channel)
{
}

std::optional<std::uint8_t> I2cMux::readByte(const std::uint8_t device_address,
                                             const std::uint8_t register_address)
{
  if (selectChannel() == core::Result::kFailure) { return std::nullopt; }
  return i2c_->readByte(device_address, register_address);
}

core::Result I2cMux::writeByte(const std::uint8_t device_address, const std::uint8_t data)
{
  if (selectChannel() == core::Result::kFailure) { return core::Result::kFailure; }
  return i2c_->writeByte(device_address, data);
}

core::Result I2cMux::writeByteToRegister(const std::uint8_t device_address,
                                         const std::uint8_t register_address,
                                         const std::uint8_t data)
{
  if (selectChannel() == core::Result::kFailure) { return core::Result::kFailure; }
  return i2c_->writeByteToRegister(device_address, register_address, data);
}

core::Result I2cMux::writeByteToRegister(const std::uint8_t device_address,
                                         const std::uint16_t register_address,
                                         const std::uint8_t data)
{
  if (selectChannel() == core::Result::kFailure) { return core::Result::kFailure; }
  return i2c_->writeByteToRegister(device_address, register_address, data);
}

core::Result I2cMux::selectChannel()
{
  const std::uint8_t channel_buffer = 1 << channel_;
  const auto i2c_write_result       = i2c_->writeByte(mux_address_, channel_buffer);
  if (i2c_write_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to select i2c mux channel %d", channel_);
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kInfo, "I2c Mux Channel %d selected", channel_);
  return core::Result::kSuccess;
}

}  // namespace hyped::sensors
