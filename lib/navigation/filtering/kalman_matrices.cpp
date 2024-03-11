#include "kalman_matrices.hpp"

namespace hyped::navigation {

// Constant matrices
const StateTransitionMatrix kStateTransitionMatrix
  = (StateTransitionMatrix() << 1, kDeltaT, 0, 1).finished();

const ControlMatrix kControlMatrix
  = (ControlMatrix() << 0.5 * kDeltaT * kDeltaT, kDeltaT).finished();

const ErrorCovarianceMatrix kErrorCovarianceMatrix
  = (ErrorCovarianceMatrix() << 1, 0, 0, 1).finished();

const MeasurementNoiseCovarianceMatrix kMeasurementNoiseCovarianceMatrix
  = (MeasurementNoiseCovarianceMatrix() << 0.01, 0, 0, 0.01).finished();

const MeasurementMatrix obs_matrix_no_keyence
  = ((MeasurementMatrix() << 0, 0, 0, kDeltaT).finished());

const MeasurementMatrix obs_matrix_with_keyence
  = ((MeasurementMatrix() << 1, 0, 0, kDeltaT).finished());

// Changing matrices
StateVector initial_state = (StateVector::Zero());

StateTransitionCovarianceMatrix initial_covariance = (StateTransitionCovarianceMatrix::Zero());

ControlInput control_input_vector = ControlInput::Zero();

MeasurementVector measurement_vector = MeasurementVector::Zero();

MeasurementMatrix measurement_matrix = ((MeasurementMatrix() << 0, 0, 0, kDeltaT).finished());

}  // namespace hyped::navigation