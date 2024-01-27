#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <Eigen/Dense>
#include <core/time.hpp>
#include <core/types.hpp>
#include "kalman_matrices.hpp"

namespace hyped::navigation {

class KalmanFilter {
 public:
  KalmanFilter(const StateVector &initial_state,
               const ErrorCovarianceMatrix &initial_error_covariance,
               const StateTransitionMatrix &transition_matrix,
               const ControlMatrix &control_matrix,
               const StateTransitionCovarianceMatrix &transition_covariance,
               const MeasurementMatrix &measurement_matrix,
               const MeasurementNoiseCovarianceMatrix &measurement_noise_covariance);

  void filter(const MeasurementVector &measurement,
              const ControlInput &control_input);

  inline const StateVector &getStateEstimate() const { return state_estimate_; }

  inline const ErrorCovarianceMatrix &getErrorCovariance() const { return error_covariance_; }

 private:
  StateVector state_estimate_;
  ErrorCovarianceMatrix error_covariance_;
  StateTransitionMatrix transition_matrix;
  ControlMatrix control_matrix;
  StateTransitionCovarianceMatrix transition_covariance;
  MeasurementMatrix measurement_matrix;
  MeasurementNoiseCovarianceMatrix measurement_noise_covariance;
};

}  // namespace hyped::navigation