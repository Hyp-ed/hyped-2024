#include "kalman_filter.hpp"

namespace hyped::navigation {

KalmanFilter::KalmanFilter(StateVector initial_state,
                           ErrorCovarianceMatrix initial_error_covariance,
                           StateTransitionMatrix transition_matrix,
                           ControlMatrix control_matrix,
                           StateTransitionCovarianceMatrix transition_covariance,
                           MeasurementMatrix measurement_matrix,
                           MeasurementNoiseCovarianceMatrix measurement_noise_covariance)
    : state_estimate_(std::move(initial_state)),
      error_covariance_(std::move(initial_error_covariance)),
      transition_matrix(std::move(transition_matrix)),
      control_matrix(std::move(control_matrix)),
      transition_covariance(std::move(transition_covariance)),
      measurement_matrix(std::move(measurement_matrix)),
      measurement_noise_covariance(std::move(measurement_noise_covariance))

{
  static_assert(state_dimension > 0);
  static_assert(measurement_dimension > 0);
}

void KalmanFilter::filter(const MeasurementVector &measurement, const ControlInput &control_input)
{
  // Set correct measurement matrix

  if (measurement.isZero()) {
    measurement_matrix = ((MeasurementMatrix() << 0, 0, 0, kDeltaT).finished());
  } else {
    measurement_matrix = ((MeasurementMatrix() << 1, 0, 0, 1).finished());
  }

  // Predict
  // x_k = Fx_k-1 + Bu_k

  state_estimate_ = transition_matrix * state_estimate_ + control_matrix * control_input;

  // P_k = F*P_k-1*F^T + Q

  error_covariance_
    = transition_matrix * error_covariance_ * transition_matrix.transpose() + transition_covariance;

  // Update

  // K_k = P_k*H^T*(H*P_k*H^T + R)^-1

  const auto kalman_gain
    = error_covariance_ * measurement_matrix.transpose()
      * (measurement_matrix * error_covariance_ * measurement_matrix.transpose()
         + measurement_noise_covariance)
          .inverse();

  // x_k = x_k + K_k*(z_k - H*x_k)

  state_estimate_
    = state_estimate_ + kalman_gain * (measurement - measurement_matrix * state_estimate_);

  // P_k = (I - K_k*H)*P_k*(I - K_k*H)^T + K_k*R*K_k^T

  const auto difference = Eigen::Matrix<core::Float, state_dimension, state_dimension>::Identity()
                          - kalman_gain * measurement_matrix;

  error_covariance_ = difference * error_covariance_ * difference.transpose()
                      + kalman_gain * measurement_noise_covariance * kalman_gain.transpose();
}

}  // namespace hyped::navigation
