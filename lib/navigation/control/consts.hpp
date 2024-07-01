#pragma once

#include <optional>

#include <Eigen/Dense>
#include <core/types.hpp>

namespace hyped::navigation {

// TODOLater: move most of these to a config file
static constexpr core::Float kTrackLength     = 6.0;  // m
static constexpr core::Float kBrakingDistance = 2.0;  // m TODOLater:check!
static constexpr core::Float kStripeDistance  = 5.0;
static constexpr bool kIsKeyenceActive        = true;

// define sensor checks return type
enum class SensorChecks { kUnacceptable = 0, kAcceptable };

struct Quartiles {
  core::Float q1;
  core::Float median;
  core::Float q3;
};

class INavigator {
 public:
  virtual std::optional<core::Trajectory> currentTrajectory()               = 0;
  virtual core::Result keyenceUpdate(const core::KeyenceData &keyence_data) = 0;
  virtual core::Result accelerometerUpdate(const core::RawAccelerometerData &accelerometer_data)
    = 0;
  virtual core::Result opticalUpdate(const core::OpticalData &optical_data) = 0;
};

}  // namespace hyped::navigation
