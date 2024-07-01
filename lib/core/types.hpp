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
static constexpr std::uint8_t kNumAccelerometers = 1;
static constexpr std::uint8_t kNumAxis           = 3;
static constexpr std::uint8_t kNumKeyence        = 1;
static constexpr std::uint8_t kNumOptical        = 1;

// data format for raw sensor data
using RawAccelerometerData = std::array<std::array<Float, kNumAxis>, kNumAccelerometers>;
using AccelerometerData    = std::array<Float, kNumAccelerometers>;
using KeyenceData          = std::array<std::uint32_t, kNumKeyence>;
using OpticalData          = std::array<std::array<Float, 2>, kNumOptical>;

// data produced by the accelerometer sensor
// values are in milli-g (standard gravity)
struct RawAccelerationData {
  std::int32_t x;
  std::int32_t y;
  std::int32_t z;
  TimePoint measured_at;
  bool is_sensor_active;
};

}  // namespace hyped::core
