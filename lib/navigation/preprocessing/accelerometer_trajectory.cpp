#include "accelerometer_trajectory.hpp"

#include <cmath>

#include <chrono>

namespace hyped::navigation {

AccelerometerTrajectoryEstimator::AccelerometerTrajectoryEstimator(const core::ITimeSource &time)
    : time_(time),
      previous_timestamp_(time.now()),
      displacement_estimate_(0),
      velocity_estimate_(0)
{
}

// TODOLater: change arguments to instead take some sort of "datapoint" struct with acc_val and
// timestamp instead, also switch to doubles?
void AccelerometerTrajectoryEstimator::update(const core::Float acceleration,
                                              const core::TimePoint timestamp)
{
  core::Timer timer_(time_);
  const auto time_elapsed                = timer_.elapsed(timestamp, previous_timestamp_);
  const core::Float time_elapsed_seconds = timer_.elapsedTimeInSeconds(time_elapsed);

  // from equation s = s_0 + ut + 0.5*a*(t^2)
  displacement_estimate_ = displacement_estimate_ + (velocity_estimate_ * time_elapsed_seconds)
                           + (0.5 * acceleration * time_elapsed_seconds * time_elapsed_seconds);

  // from equation v=u+at
  velocity_estimate_  = velocity_estimate_ + (acceleration * time_elapsed_seconds);
  previous_timestamp_ = timestamp;
}

}  // namespace hyped::navigation