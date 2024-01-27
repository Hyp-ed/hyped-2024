#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <Eigen/Dense>
#include <core/time.hpp>
#include <core/types.hpp>
#include <navigation/control/consts.hpp>

namespace hyped::navigation {

class KalmanFilter {
 public:
  KalmanFilter(std::shared_ptr<core::ITimeSource> time_source,
               const StateVector initial_state,
               const ErrorCovarianceMatrix initial_error_covariance,
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
  std::shared_ptr<core::ITimeSource> time_source_;
  core::TimePoint last_update_time_;
  StateVector state_estimate_;
  ErrorCovarianceMatrix error_covariance_;
  StateTransitionMatrix transition_matrix;
  ControlMatrix control_matrix;
  StateTransitionCovarianceMatrix transition_covariance;
  MeasurementMatrix measurement_matrix;
  MeasurementNoiseCovarianceMatrix measurement_noise_covariance;
  
};

}  // namespace hyped::navigation