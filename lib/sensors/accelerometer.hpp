#pragma once

#include "i2c_sensors.hpp"

#include <unistd.h>

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <core/wall_clock.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

constexpr std::string_view kAxisLabels[3] = {"x-axis", "y-axis", "z-axis"};

// possible addresses for the accelerometer (from the datasheet)
constexpr std::uint8_t kDefaultAccelerometerAddress     = 0x19;
constexpr std::uint8_t kAlternativeAccelerometerAddress = 0x18;

class Accelerometer : public II2cMuxSensor<core::RawAccelerationData> {
 public:
  static std::optional<Accelerometer> create(core::ILogger &logger,
                                             std::shared_ptr<io::II2c> i2c,
                                             const std::uint8_t channel,
                                             const std::uint8_t device_address);
  ~Accelerometer();

  /*
   * @brief Checks if the accelerometer is ready to be read
   * @return kSuccess if the sensor is ready to be read,
   *         kFailure if the sensor is not ready to be read,
   *         nullopt if there was an error reading the sensor
   */
  std::optional<core::Result> isValueReady();

  std::optional<core::RawAccelerationData> read();

  std::uint8_t getChannel() const;

 private:
  Accelerometer(core::ILogger &logger,
                std::shared_ptr<io::II2c> i2c,
                const std::uint8_t channel,
                const std::uint8_t device_address);
  std::optional<std::int16_t> getRawAcceleration(const core::Axis axis);
  std::int32_t getAccelerationFromRawValue(const std::int16_t rawAcceleration);
  void setRegisterAddressFromAxis(const core::Axis axis);

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t channel_;
  std::uint8_t low_byte_address_;
  std::uint8_t high_byte_address_;
  const std::uint8_t device_address_;

 private:
  // Register addresses/values taken from the datasheet
  static constexpr std::uint8_t kCtrl1Address = 0x20;
  // Sampling rate of 200 Hz
  // Enable high performance mode
  static constexpr std::uint8_t kCtrl1Value   = 0x64;
  static constexpr std::uint8_t kCtrl2Address = 0x21;
  // Enable block data update
  // Enable address auto increment
  static constexpr std::uint8_t kCtrl2Value   = 0x0;
  static constexpr std::uint8_t kCtrl6Address = 0x25;
  // Full scale +-16g
  // Filter bandwidth = ODR/2
  static constexpr std::uint8_t kCtrl6Value = 0x30;
  // Register addresses for the x, y, and z axis
  static constexpr std::uint8_t kXOutLow  = 0x28;
  static constexpr std::uint8_t kXOutHigh = 0x29;

  static constexpr std::uint8_t kYOutLow  = 0x2A;
  static constexpr std::uint8_t kYOutHigh = 0x2B;

  static constexpr std::uint8_t kZOutLow  = 0x2C;
  static constexpr std::uint8_t kZOutHigh = 0x2D;

  static constexpr std::uint8_t kDataReady             = 0x27;
  static constexpr std::uint8_t kDeviceIdAddress       = 0x0F;
  static constexpr std::uint8_t kExpectedDeviceIdValue = 0x44;
};

}  // namespace hyped::sensors