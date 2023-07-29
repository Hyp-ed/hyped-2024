#include <numeric>

#include <gtest/gtest.h>

#include <core/types.hpp>
#include <utils/naive_navigator.hpp>

namespace hyped::test {

// TODOLater: improve testing method here!
void testWithTrajectory(utils::NaiveNavigator &naive_navigator,
                        const core::RawAccelerometerData accelerometer_data,
                        const core::EncoderData encoder_data,
                        const core::KeyenceData keyence_data)
{
  naive_navigator.accelerometerUpdate(accelerometer_data);
  naive_navigator.encoderUpdate(encoder_data);
  naive_navigator.keyenceUpdate(keyence_data);

  core::Float sum_accelerometer = 0;
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    for (std::size_t j = 0; j < core::kNumAxis; ++j) {
      sum_accelerometer += accelerometer_data.at(i).at(j);
    }
  }
  core::Float new_acceleration = sum_accelerometer / core::kNumAccelerometers;
  core::Float new_velocity     = 0;
  core::Float new_displacement
    = std::accumulate(encoder_data.begin(), encoder_data.end(), 0.0) / core::kNumEncoders;

  const auto &current_trajectory = naive_navigator.currentTrajectory();
  if (current_trajectory.has_value()) {
    ASSERT_FLOAT_EQ(current_trajectory.value().acceleration, new_acceleration);
    ASSERT_FLOAT_EQ(current_trajectory.value().velocity, new_velocity);
    ASSERT_FLOAT_EQ(current_trajectory.value().displacement, new_displacement);
  }
}

TEST(NaiveNavigator, basic)
{
  utils::NaiveNavigator naive_navigator;
  testWithTrajectory(naive_navigator,
                     {{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}},
                     {0, 0, 0, 0},
                     {0, 0});
  testWithTrajectory(
    naive_navigator, {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}}}, {1, 2, 3, 4}, {1, 2});
  testWithTrajectory(
    naive_navigator, {{{10, 5, 1}, {0, 100, 3}, {2, 3, 2}, {50, 1, 200}}}, {20, 30, 1, 5}, {30, 2});
  testWithTrajectory(
    naive_navigator, {{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}}}, {1, 1, 1, 1}, {1, 1});
}

}  // namespace hyped::test
