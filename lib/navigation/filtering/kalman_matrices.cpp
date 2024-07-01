#include "kalman_matrices.hpp"

namespace hyped::navigation {
// TODOLater: this entire thing is awful, fix it

// Constant matrices
const StateTransitionMatrix kStateTransitionMatrix
  = (StateTransitionMatrix() << 1, kDeltaT, 0, 1).finished();

const ControlMatrix kControlMatrix
  = (ControlMatrix() << 0.5 * kDeltaT * kDeltaT, kDeltaT).finished();

// Assuming frequency of 6400hz for IMU at 120 mu g / sqrt(Hz)
// standard deviation = 120 * sqrt(6400) = 9600 mu g = 0.0096 g
//                    = 0.0096 * 9.81 = 0.094176 m/s^2
// variance =  0.094176^2 = 0.0089 m/s^2

const StateTransitionCovarianceMatrix transition_covariance
  = (StateTransitionCovarianceMatrix() << 0.25 * std::pow(kDeltaT, 4),
     0.5 * std::pow(kDeltaT, 3),
     0.5 * std::pow(kDeltaT, 3),
     std::pow(kDeltaT, 2))
      .finished()
    * 0.0089;

// asumming stripe counter is accurate

// Optical flow expects standard deviation of 0.01% of the measured value
//  Assuming top speed 10m/s,
//  standard deviation = 0.01 * 10 = 0.1 m/s
//  variance = 0.1^2 = 0.01 m/s^2

const MeasurementNoiseCovarianceMatrix kMeasurementNoiseCovarianceMatrix
  = (MeasurementNoiseCovarianceMatrix() << 0.01, 0, 0, 0).finished();

const ErrorCovarianceMatrix kErrorCovarianceMatrix
  = (ErrorCovarianceMatrix() << 0.01, 0, 0, 0.01).finished();

// Changing matrices
StateVector initial_state = (StateVector::Zero());

ErrorCovarianceMatrix initial_covariance = (ErrorCovarianceMatrix::Zero());

ControlInput control_input_vector = ControlInput::Zero();

MeasurementVector measurement_vector = MeasurementVector::Zero();

MeasurementMatrix measurement_matrix = ((MeasurementMatrix() << 0, 0, 0, kDeltaT).finished());

}  // namespace hyped::navigation
