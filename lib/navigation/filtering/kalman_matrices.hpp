#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include <Eigen/Dense>
#include <core/types.hpp>

namespace hyped::navigation {

constexpr std::size_t state_dimension       = 2;
constexpr std::size_t control_dimension     = 1;
constexpr std::size_t measurement_dimension = 2;
constexpr core::Float kDeltaT               = 0.01;

// Constant matrices
using StateTransitionMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
extern const StateTransitionMatrix kStateTransitionMatrix;

using ControlMatrix = Eigen::Matrix<core::Float, state_dimension, control_dimension>;
extern const ControlMatrix kControlMatrix;

using ErrorCovarianceMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
extern const ErrorCovarianceMatrix kErrorCovarianceMatrix;

using StateTransitionCovarianceMatrix
  = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
extern StateTransitionCovarianceMatrix process_noise_covariance;
using MeasurementNoiseCovarianceMatrix
  = Eigen::Matrix<core::Float, measurement_dimension, measurement_dimension>;
extern const MeasurementNoiseCovarianceMatrix kMeasurementNoiseCovarianceMatrix;

// Changing matrices
using StateVector = Eigen::Matrix<core::Float, state_dimension, 1>;
extern StateVector initial_state;


using ControlInput = Eigen::Matrix<core::Float, control_dimension, 1>;
extern ControlInput control_input_vector;
using MeasurementVector = Eigen::Matrix<core::Float, measurement_dimension, 1>;
extern MeasurementVector measurement_vector;

// Not const because changes depending on whether keyence data is available
using MeasurementMatrix = Eigen::Matrix<core::Float, measurement_dimension, state_dimension>;
extern MeasurementMatrix measurement_matrix;

}  // namespace hyped::navigation