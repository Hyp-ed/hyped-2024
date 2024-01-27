#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include <Eigen/Dense>
#include <core/types.hpp>

namespace hyped::navigation {

constexpr std::size_t state_dimension       = 3;
constexpr std::size_t measurement_dimension = 1;
// in order acc, velocity, displacement
using StateVector           = Eigen::Matrix<core::Float, state_dimension, 1>;
using StateTransitionMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
using StateTransitionCovarianceMatrix
  = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
using ErrorCovarianceMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
using MeasurementMatrix
  = Eigen::Matrix<core::Float, measurement_dimension, state_dimension>;  //[[1, 0, 0], [0, 0, 0],
                                                                         //[0, 0, 0]]
using MeasurementVector = Eigen::Matrix<core::Float, measurement_dimension, 1>;  //[acc_val, 0, 0]
using MeasurementNoiseCovarianceMatrix
  = Eigen::Matrix<core::Float, measurement_dimension, measurement_dimension>;

// TODOLater: move most of these to a config file
static constexpr core::Float kTrackLength     = 100.0;  // m
static constexpr core::Float kBrakingDistance = 20.0;   // m TODOLater:check!
static constexpr core::Float kStripeDistance  = 5.0;
static constexpr bool kIsKeyenceActive        = true;

// define sensor checks return type
enum class SensorChecks { kUnacceptable = 0, kAcceptable };

struct Quartiles {
  core::Float q1;
  core::Float median;
  core::Float q3;
};

inline core::Trajectory zero_trajectory = {0, 0, 0};

class INavigator {
 public:
  virtual std::optional<core::Trajectory> currentTrajectory()               = 0;
  virtual core::Result keyenceUpdate(const core::KeyenceData &keyence_data) = 0;
  virtual core::Result accelerometerUpdate(const core::RawAccelerometerData &accelerometer_data)
    = 0;
};
}  // namespace hyped::navigation