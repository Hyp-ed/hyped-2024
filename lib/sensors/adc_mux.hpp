#pragma once

#include "adc_sensors.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

namespace hyped::adc {

constexpr std::uint8_t kDefaultMuxAddress;
constexpr std::uint8_t kMaxNumMuxChannels = 8;
constexpr core::Float kFailureThreshold   = 0.25;

template<class T, std::uint8_t N>
class AdcMux {
 public:
  AdcMux(core::ILogger &logger,
         std::shared_ptr<io::II2c> i2c,
         const std::uint8_t mux_address,
         std::array<std::unique_ptr<IAdcMuxSensor<T>>, N> &channels);
  ~AdcMux();

  std::optional<std::array<T, N>> readData();

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t mux_address_;
  const std::array<std::unique_ptr<IAdcMuxSensor<T>>, N> channels_;
  const std::uint8_t max_num_faulty_channels_;
};

template<typename T, std::uint8_t N>
AdcMux<T, N>::AdcMux(core::ILogger &logger,
                     std::shared_ptr<io::II2c> i2c,
                     const std::uint8_t mux_address,
                     std::array<std::unique_ptr<IAdcMuxSensor<T>>, N> &channels)
    : logger_(logger),
      i2c_(i2c),
      mux_address_(mux_address),
      channels_(std::move(channels)),
      max_num_faulty_channels_(static_cast<std::uint8_t>(kFailureThreshold * N))
{
  static_assert(N <= 8, "The ADC mux can only have up to 8 channels");
}

template<typename T, std::uint8_t N>
AdcMux<T, N>::~AdcMux()
{
}

template<typename T, std::uint8_t N>
std::optional<std::array<T, N>> AdcMux<T, N>::readData()
{
  std::array<T, N> mux_data{};
  std::uint8_t num_unusable_sensors = 0;
  for (std::uint8_t i = 0; i < N; ++i) {
    const auto &sensor            = sensors.at(i);
    const std::uint8_t sensor_reg = sensor->getRegister();
    const auto sensor_data        = sensor->read(sensor_reg) if (!sensor_data)
    {
      logger_.log(core::LogLevel::kFatal, "Failed to get i2c mux data from channel %d", channel);
      ++num_unusable_sensors;
      continue;
    }
    mux_data.at(i) = *sensor_data;
    if (num_unusable_sensors > max_num_unusable_sensors_) {
      logger_.log(core::LogLevel::kFatal,
                  "Failed to read from more than %0.f%% of sensors on the i2c mux",
                  kFailureThreshold * 100);
      return std::nullopt;
    }
    return mux_data;
  }
}

}  // namespace hyped::adc
