#include <iostream>

#include <gtest/gtest.h>

#include <navigation/control/consts.hpp>
#include <navigation/filtering/kalman_filter.hpp>
#include <navigation/filtering/kalman_matrices.hpp>
#include <utils/manual_time.hpp>

namespace hyped::test {


//Done for delta_t = 1

TEST(KalmanFilter, construction)
{
  using KalmanFilter     = navigation::KalmanFilter;
  const auto manual_time = std::make_shared<utils::ManualTime>();


  navigation::StateVector initial_state(navigation::StateVector::Zero());

  navigation::ErrorCovarianceMatrix initial_error_covariance;
  initial_error_covariance << 400, 0, 0, 400;

  navigation::StateTransitionMatrix transition_matrix;
  transition_matrix << 1, 1, 0, 1;

  navigation::ControlMatrix control_matrix;
  control_matrix << 0.5, 1;

  navigation::MeasurementNoiseCovarianceMatrix process_noise_covariance;
  process_noise_covariance << 1/4, 1/2, 1/2, 1;

  navigation::MeasurementMatrix measurement_matrix;
  measurement_matrix << 1, 0, 0, 1;

  navigation::MeasurementNoiseCovarianceMatrix measurement_noise_covariance;
  measurement_noise_covariance << 0, 0, 0, 1;

  KalmanFilter kalman_filter(initial_state,
                             initial_error_covariance,
                             transition_matrix,
                             control_matrix,
                             process_noise_covariance,
                             measurement_matrix,
                             measurement_noise_covariance);

  navigation::MeasurementVector measurement;
  navigation::ControlInput control_input(navigation::ControlInput::Zero());


  navigation::StateVector state_estimate;

  state_estimate = kalman_filter.getStateEstimate();

  std::cout << "State estimate: " << state_estimate << std::endl;

  measurement << 1, 0;

  
  kalman_filter.filter(measurement, control_input);
  

  state_estimate = kalman_filter.getStateEstimate();

  std::cout << "State estimate: " << state_estimate << std::endl;
}
}  // namespace hyped::test

// namespace hyped::test
