#include <iostream>

#include <gtest/gtest.h>

#include <navigation/control/consts.hpp>
#include <navigation/filtering/kalman_filter.hpp>
#include <navigation/filtering/kalman_matrices.hpp>
#include <utils/manual_time.hpp>

namespace hyped::test {

TEST(KalmanFilter, construction)
{
  using KalmanFilter     = navigation::KalmanFilter;
  const auto manual_time = std::make_shared<utils::ManualTime>();
  navigation::StateVector initial_state(navigation::StateVector::Zero());
  navigation::ErrorCovarianceMatrix initial_error_covariance;
  KalmanFilter kalman_filter(
    initial_state,
    navigation::ErrorCovarianceMatrix::Zero(),  // If initial position not known exactly, tune
    navigation::kStateTransitionMatrix,
    navigation::kControlMatrix,
    navigation::kErrorCovarianceMatrix,
    navigation::measurement_matrix,
    navigation::kMeasurementNoiseCovarianceMatrix);

  /*
  kalman_filter.filter(KalmanFilter::StateTransitionMatrix(),
                       KalmanFilter::StateTransitionCovarianceMatrix(),
                       KalmanFilter::MeasurementMatrix(),
                       KalmanFilter::MeasurementNoiseCovarianceMatrix(),
                       KalmanFilter::StateVector());
 */
}

}  // namespace hyped::test
