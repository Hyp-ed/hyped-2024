#include "kalman_matrices.hpp"

namespace hyped::navigation {

StateVector initial_state = (StateVector::Zero());

StateTransitionCovarianceMatrix initial_covariance = (StateTransitionCovarianceMatrix::Zero());

MeasurementMatrix measurement_matrix = ((MeasurementMatrix() << 0, 0,
                                                                0, kDeltaT).finished());
ControlInput control_input_vector = ControlInput::Zero();

MeasurementVector measurement_vector = MeasurementVector::Zero();

} // namespace hyped::navigation