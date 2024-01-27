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
const StateTransitionMatrix kStateTransitionMatrix((StateTransitionMatrix() << 1, kDeltaT, 
                                                                               0, 1).finished());

using ControlMatrix = Eigen::Matrix<core::Float, state_dimension, control_dimension>;
const ControlMatrix kControlMatrix((ControlMatrix() << 0.5 * kDeltaT * kDeltaT, 
                                                       kDeltaT).finished());

//TODO: tune this
using ErrorCovarianceMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
const ErrorCovarianceMatrix kErrorCovarianceMatrix((ErrorCovarianceMatrix() << 1, 0,
                                                                               0, 1).finished());

//TODO: tune this
using MeasurementNoiseCovarianceMatrix = Eigen::Matrix<core::Float, measurement_dimension, measurement_dimension>;
const MeasurementNoiseCovarianceMatrix kMeasurementNoiseCovarianceMatrix((MeasurementNoiseCovarianceMatrix() << 0.01, 0, 
                                                                                                                0, 0.01).finished());

//Changing matrices

using StateVector = Eigen::Matrix<core::Float, state_dimension, 1>;
extern StateVector initial_state;
using StateTransitionCovarianceMatrix
  = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
extern StateTransitionCovarianceMatrix initial_covariance;

//Not const because changes depending on whether keyence data is available
using MeasurementMatrix = Eigen::Matrix<core::Float, measurement_dimension, state_dimension>;
extern MeasurementMatrix measurement_matrix;

using ControlInput = Eigen::Matrix<core::Float, control_dimension, 1>;
extern ControlInput control_input_vector;
using MeasurementVector = Eigen::Matrix<core::Float, measurement_dimension, 1>;
extern MeasurementVector measurement_vector;

}  // namespace hyped::navigation