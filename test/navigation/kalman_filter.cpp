#include <iostream>
#include <random>

#include <gtest/gtest.h>

#include <navigation/control/consts.hpp>
#include <navigation/filtering/kalman_filter.hpp>
#include <navigation/filtering/kalman_matrices.hpp>
#include <utils/manual_time.hpp>

namespace hyped::test {

TEST(KalmanFilter, construction)

// Test simulating simple cart movement

// Acceleration: 20ms^-2
// Initial velocity: 0
// Initial position: 0
// Measurments taken every 1s

// Variacnce of measurments:
//  Distance: 10
//  Velocity: 5
//  Acceleration: 3

{
  using KalmanFilter     = navigation::KalmanFilter;
  const auto manual_time = std::make_shared<utils::ManualTime>();

  navigation::StateVector initial_state(navigation::StateVector::Zero());

  navigation::ErrorCovarianceMatrix initial_error_covariance;
  initial_error_covariance << 1, 0, 0, 1;

  navigation::StateTransitionMatrix transition_matrix;
  transition_matrix << 1, 1, 0, 1;

  navigation::ControlMatrix control_matrix;
  control_matrix << 0.5, 1;

  navigation::StateTransitionCovarianceMatrix process_noise_covariance;
  process_noise_covariance << 0.25 * 3, 0.5 * 3, 0.5 * 3, 1 * 3;

  navigation::MeasurementMatrix measurement_matrix;
  measurement_matrix << 1, 0, 0, 1;

  navigation::MeasurementNoiseCovarianceMatrix measurement_noise_covariance;
  measurement_noise_covariance << 3, 0, 0, 3;

  KalmanFilter kalman_filter(initial_state,
                             initial_error_covariance,
                             transition_matrix,
                             control_matrix,
                             process_noise_covariance,
                             measurement_matrix,
                             measurement_noise_covariance);

  navigation::MeasurementVector measurement;
  navigation::ControlInput control_input(navigation::ControlInput::Zero());

  // Noisy measurements

  std::array<double, 20> acc_measurements
    = {20.38, 15.02, 19.17, 19.36, 20.6,  16.89, 20.42, 20.33, 21.53, 19.98,
       26.08, 17.63, 21.01, 17.06, 21.83, 23.18, 26.03, 17.67, 22.99, 18.78};

  std::array<double, 20> dist_measurements
    = {28.59,   51.7,    87.64,   169.39,  244.96,  363.2,   493.0,  626.97,  817.84,  1021.23,
       1192.21, 1423.34, 1689.46, 1964.72, 2261.09, 2565.47, 2902.0, 3239.25, 3627.35, 4011.47};

  std::array<double, 20> vel_measurements
    = {20.06,  42.43,  66.23,  83.03,  104.47, 122.93, 135.85, 157.55, 179.02, 201.75,
       216.53, 240.63, 262.97, 285.75, 307.43, 321.04, 331.48, 354.47, 374.19, 394.29};

  // Actual measurements

  std::array<double, 20> actual_distances
    = {10,   40,   90,   160,  250,  360,  490,  640,  810,  1000,
       1210, 1440, 1690, 1960, 2250, 2560, 2890, 3240, 3610, 4000};

  std::array<double, 20> actual_velocities = {20,  40,  60,  80,  100, 120, 140, 160, 180, 200,
                                              220, 240, 260, 280, 300, 320, 340, 360, 380, 400};

  double dist_error_sum = 0.0;
  double vel_error_sum  = 0.0;

  navigation::StateVector state_estimate;

  state_estimate = kalman_filter.getStateEstimate();

  std::cout << "State estimate: " << state_estimate << std::endl;

  for (int i = 0; i < 20; i++) {
    measurement << dist_measurements[i], vel_measurements[i];
    control_input << acc_measurements[i];
    kalman_filter.filter(measurement, control_input);
    state_estimate = kalman_filter.getStateEstimate();

    // Add error

    double dist_error = state_estimate[0] - actual_distances[i];
    double vel_error  = state_estimate[1] - actual_velocities[i];

    dist_error_sum += dist_error * dist_error;
    vel_error_sum += vel_error * vel_error;
  }

  std::cout << "State estimate: " << state_estimate << std::endl;

  // Calculate RMSE

  double dist_rmse = std::sqrt(dist_error_sum / 20);
  double vel_rmse  = std::sqrt(vel_error_sum / 20);

  std::cout << "Distance RMSE: " << dist_rmse << std::endl;
  std::cout << "Velocity RMSE: " << vel_rmse << std::endl;
}
}  // namespace hyped::test

// namespace hyped::test
