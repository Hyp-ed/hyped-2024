#include "naive_navigator.hpp"

#include <numeric>

namespace hyped::utils {

NaiveNavigator::NaiveNavigator() : current_trajectory_{}
{
}

std::optional<core::Trajectory> NaiveNavigator::currentTrajectory()
{
  return current_trajectory_;
}

core::Result NaiveNavigator::keyenceUpdate(const core::KeyenceData &keyence_data)
{
  // Do nothing. Keyence has no direct influence on trajectory
  return core::Result::kSuccess;
}

core::Result NaiveNavigator::encoderUpdate(const core::EncoderData &encoder_data)
{
  core::Float sum                  = std::accumulate(encoder_data.begin(), encoder_data.end(), 0.0);
  core::Float encoder_average      = static_cast<core::Float>(sum / core::kNumEncoders);
  current_trajectory_.displacement = encoder_average;
  return core::Result::kSuccess;
}

core::Result NaiveNavigator::accelerometerUpdate(
  const core::RawAccelerometerData &accelerometer_data)
{
  core::Float sum = 0.0;
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    for (std::size_t j = 0; j < core::kNumAxis; ++j) {
      sum += accelerometer_data.at(i).at(j);
    }
  }
  core::Float accelerometer_average = static_cast<core::Float>(sum / core::kNumAccelerometers);
  current_trajectory_.acceleration  = accelerometer_average;
  // TODOLater: improve this...
  current_trajectory_.velocity = 0;
  return core::Result::kSuccess;
}

}  // namespace hyped::utils
