#pragma once

#include "i2c_sensors.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

constexpr std::uint8_t kDefaultMuxAddress = 0x70;
constexpr std::uint8_t kMaxNumMuxChannels = 8;
// percentage of sensors that can be unusable before the mux is considered unusable
constexpr core::Float kFailureThreshold = 0.25;  // TODOLater: finalize this value with Electronics

/**
 * @brief Mux for sensors using I2C
 * @details Sensors include: Temperature (Wurth), Accelerometer (Wurth), Pressure (TODOLater: add
 * name)
 */
template<class T, std::uint8_t N>
class I2cMux {
 public:
  I2cMux(core::ILogger &logger,
         std::shared_ptr<io::II2c> i2c,
         const std::uint8_t mux_address,
         std::array<std::unique_ptr<II2cMuxSensor<T>>, N> &sensors);
  ~I2cMux();

  std::optional<std::array<T, N>> readAllChannels();

 private:
  core::Result selectChannel(const std::uint8_t channel);
  core::Result closeAllChannels();

  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t mux_address_;
  const std::array<std::unique_ptr<II2cMuxSensor<T>>, N> sensors_;
  const std::uint8_t max_num_unusable_sensors_;
};

template<typename T, std::uint8_t N>
I2cMux<T, N>::I2cMux(core::ILogger &logger,
                     std::shared_ptr<io::II2c> i2c,
                     const std::uint8_t mux_address,
                     std::array<std::unique_ptr<II2cMuxSensor<T>>, N> &sensors)
    : logger_(logger),
      i2c_(i2c),
      mux_address_(mux_address),
      sensors_(std::move(sensors)),
      max_num_unusable_sensors_(static_cast<std::uint8_t>(kFailureThreshold * N))
{
  static_assert(N <= 8, "The I2c mux can only have up to 8 channels");
}

template<typename T, std::uint8_t N>
I2cMux<T, N>::~I2cMux()
{
}

template<typename T, std::uint8_t N>
std::optional<std::array<T, N>> I2cMux<T, N>::readAllChannels()
{
  // Zero-initialize the array
  std::array<T, N> mux_data{};
  std::uint8_t num_unusable_sensors = 0;
  for (std::uint8_t i = 0; i < N; ++i) {
    const auto &sensor         = sensors_.at(i);
    const std::uint8_t channel = sensor->getChannel();
    // First ensure correct channel is selected
    const auto channel_select_result = selectChannel(channel);
    if (channel_select_result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to select channel %d from the i2c mux", channel);
      return std::nullopt;
    }
    // Then read sensor data
    const auto sensor_data = sensor->read();
    if (!sensor_data) {
      logger_.log(core::LogLevel::kFatal, "Failed to get i2c mux data from channel %d", channel);
      ++num_unusable_sensors;
      continue;
    }
    mux_data.at(i)                    = *sensor_data;
    const auto closing_channel_result = closeAllChannels();
    if (closing_channel_result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to close all i2c mux channels while reading");
      return std::nullopt;
    }
  }
  // If too many sensors are unusable, assume mux is faulty
  if (num_unusable_sensors > max_num_unusable_sensors_) {
    logger_.log(core::LogLevel::kFatal,
                "Failed to read from more than %0.f%% of sensors on the i2c mux",
                kFailureThreshold * 100);
    return std::nullopt;
  }
  return mux_data;
}

template<typename T, std::uint8_t N>
core::Result I2cMux<T, N>::selectChannel(const std::uint8_t channel)
{
  if (channel >= kMaxNumMuxChannels) {
    logger_.log(core::LogLevel::kFatal, "I2c Mux Channel number %d is not selectable", channel);
    return core::Result::kFailure;
  }
  const std::uint8_t channel_buffer = 1 << channel;
  const auto i2c_write_result       = i2c_->writeByte(mux_address_, channel_buffer);
  if (i2c_write_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to select i2c mux channel %d", channel);
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kInfo, "I2c Mux Channel %d selected", channel);
  return core::Result::kSuccess;
}

template<typename T, std::uint8_t N>
core::Result I2cMux<T, N>::closeAllChannels()
{
  const std::uint8_t clear_channel_buffer = 0x00;
  const auto i2c_write_result             = i2c_->writeByte(mux_address_, clear_channel_buffer);
  if (i2c_write_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to close all i2c mux channels");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kInfo, "All i2c mux channels closed");
  return core::Result::kSuccess;
}

}  // namespace hyped::sensors
