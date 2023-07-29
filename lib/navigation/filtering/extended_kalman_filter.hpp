#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <core/time.hpp>
#include <core/types.hpp>

#if defined(__linux__)
#include <eigen3/Eigen/Dense>
//#else
// TODO: add appropriate eigen include for mac here
#endif

namespace hyped::navigation {

// State dimension 3, measurement dimension 1, extended dimension 2
template<std::size_t state_dimension,
         std::size_t measurement_dimension,
         std::size_t extended_dimension>
class ExtendedKalmanFilter {
 public:
  ExtendedKalmanFilter(std::shared_ptr<core::ITimeSource> time_source,
                       const StateVector initial_state,
                       const ErrorCovarianceMatrix initial_error_covariance)
      : time_source_(time_source),
        last_update_time_(time_source->now()),
        state_estimate_(initial_state),
        error_covariance_(initial_error_covariance)
  {
    static_assert(state_dimension > 0);
    static_assert(measurement_dimension > 0);
    static_assert(extended_dimension > 0);
  }

  void filter(const StateTransitionMatrix &transition_matrix,
              const StateTransitionCovarianceMatrix &transition_covariance,
              const MeasurementMatrix &measurement_matrix,
              const MeasurementNoiseCovarianceMatrix &measurement_noise_covariance,
              const MeasurementVector &measurement,
              const JacobianMatrix &jacobian_matrix,
              const ExtendedStateVector &extended_state_vector)
  {
    // TODOLater: add noise to propogation step
    const auto prop_state_estimate
      = transition_matrix * state_estimate_ + jacobian_matrix * extended_state_vector;
    const auto prop_error_covariance
      = (transition_matrix.transpose() * error_covariance_ * transition_matrix)
        + transition_covariance;
    const auto kalman_gain
      = prop_error_covariance * measurement_matrix.transpose()
        * (measurement_matrix * prop_error_covariance * measurement_matrix.transpose()
           + measurement_noise_covariance)
            .inverse();
    state_estimate_ = prop_state_estimate
                      + kalman_gain * (measurement - measurement_matrix * prop_state_estimate);
  }

  const StateVector &getStateEstimate() const { return state_estimate_; }
  const ErrorCovarianceMatrix &getErrorCovariance() const { return error_covariance_; }

 private:
  std::shared_ptr<core::ITimeSource> time_source_;
  core::TimePoint last_update_time_;
  StateVector state_estimate_;
  ErrorCovarianceMatrix error_covariance_;
};

}  // namespace hyped::navigation