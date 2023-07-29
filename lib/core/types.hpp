#pragma once

#include "time.hpp"

#include <array>
#include <cstdint>

namespace hyped::core {

static constexpr float kEpsilon = 0.0001;

enum class DigitalSignal { kLow = 0, kHigh };
enum class Result { kSuccess = 0, kFailure };
enum class Axis { kX = 0, kY, kZ };

using Float = float;

// current trajectory struct
struct Trajectory {
  Float displacement;
  Float velocity;
  Float acceleration;
};

// number of each type of sensors
static constexpr std::uint8_t kNumAccelerometers = 4;
static constexpr std::uint8_t kNumAxis           = 3;
static constexpr std::uint8_t kNumEncoders       = 4;
static constexpr std::uint8_t kNumKeyence        = 2;

// data format for raw sensor data
using RawAccelerometerData = std::array<std::array<Float, kNumAxis>, kNumAccelerometers>;
using AccelerometerData    = std::array<Float, kNumAccelerometers>;
using EncoderData          = std::array<std::uint32_t, kNumEncoders>;
using KeyenceData          = std::array<std::uint32_t, kNumKeyence>;

// data produced by the accelerometer sensor
// values are in milli-g (standard gravity)
struct RawAccelerationData {
  RawAccelerationData(const std::int32_t x,
                      const std::int32_t y,
                      const std::int32_t z,
                      const TimePoint measured_at,
                      const bool is_sensor_active)
      : x(x),
        y(y),
        z(z),
        measured_at(measured_at),
        is_sensor_active(is_sensor_active)

  {
  }

  const std::int32_t x;
  const std::int32_t y;
  const std::int32_t z;
  const TimePoint measured_at;
  const bool is_sensor_active;
};

}  // namespace hyped::core
