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
const core::Float kDeltaT = 0.01;


//Constant matrices

using StateTransitionMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
const StateTransitionMatrix kStateTransitionMatrix = (StateTransitionMatrix() << 1, kDeltaT, 0, 1).finished();

using ControlMatrix = Eigen::Matrix<core::Float, state_dimension, control_dimension>;
const ControlMatrix kControlMatrix = (ControlMatrix() << 0.5 * kDeltaT * kDeltaT, kDeltaT).finished();

using ErrorCovarianceMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
const ErrorCovarianceMatrix kErrorCovarianceMatrix = (ErrorCovarianceMatrix() << 1, 0, 0, 1).finished();


using MeasurementMatrix = Eigen::Matrix<core::Float, measurement_dimension, state_dimension>;
const MeasurementMatrix kMeasurementMatrix_optical_only = (MeasurementMatrix() << 0, 0, 0, kDeltaT).finished();
const MeasurementMatrix kMeasurementMatrix_both = (MeasurementMatrix() << 1, 0, 0, kDeltaT).finished();

using MeasurementNoiseCovarianceMatrix = Eigen::Matrix<core::Float, measurement_dimension, measurement_dimension>;
const MeasurementNoiseCovarianceMatrix kMeasurementNoiseCovarianceMatrix = (MeasurementNoiseCovarianceMatrix() << 0.01, 0, 0, 0.01).finished();

//Changing values
using StateVector = Eigen::Matrix<core::Float, state_dimension, 1>;
using StateTransitionCovarianceMatrix
  = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
using ControlInput = Eigen::Matrix<core::Float, control_dimension, 1>;
using MeasurementVector = Eigen::Matrix<core::Float, measurement_dimension, 1>;

}  // namespace hyped::navigation