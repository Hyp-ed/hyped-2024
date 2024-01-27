#include "kalman_filter.hpp"

namespace hyped::navigation {

KalmanFilter::KalmanFilter(const StateVector initial_state,
                           const ErrorCovarianceMatrix initial_error_covariance,
                           const StateTransitionMatrix &transition_matrix,
                           const ControlMatrix &control_matrix,
                           const StateTransitionCovarianceMatrix &transition_covariance,
                           const MeasurementMatrix &measurement_matrix,
                           const MeasurementNoiseCovarianceMatrix &measurement_noise_covariance)
    : state_estimate_(initial_state),
      error_covariance_(initial_error_covariance),
      transition_matrix(transition_matrix),
      control_matrix(control_matrix),
      transition_covariance(transition_covariance),
      measurement_matrix(measurement_matrix),
      measurement_noise_covariance(measurement_noise_covariance)
{
  static_assert(state_dimension > 0);
  static_assert(measurement_dimension > 0);
}

void KalmanFilter::filter(const MeasurementVector &measurement,
                          const ControlInput &control_input)
{
  const auto prop_state_estimate = transition_matrix * state_estimate_ + control_matrix * control_input;
  const auto prop_error_covariance
    = (transition_matrix.transpose() * error_covariance_ * transition_matrix)
      + transition_covariance;

  const auto repeated_matrix = prop_error_covariance * measurement_matrix.transpose();
  const auto kalman_gain
    = repeated_matrix
      * (measurement_matrix * repeated_matrix + measurement_noise_covariance).inverse();
  state_estimate_
    = prop_state_estimate + kalman_gain * (measurement - measurement_matrix * prop_state_estimate);
}

}  // namespace hyped::navigation