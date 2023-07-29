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

  // from equation v=u+at
  const core::Float velocity_estimate = velocity_estimate_ + (acceleration * time_elapsed_seconds);

  // from equation s = ut + 0.5*a*(t^2)
  displacement_estimate_ = (velocity_estimate_ * time_elapsed_seconds)
                           + (0.5 * acceleration * time_elapsed_seconds * time_elapsed_seconds);
  velocity_estimate_  = velocity_estimate;
  previous_timestamp_ = timestamp;
}

core::Float AccelerometerTrajectoryEstimator::getDisplacementEstimate() const
{
  return displacement_estimate_;
}

core::Float AccelerometerTrajectoryEstimator::getVelocityEstimate() const
{
  return velocity_estimate_;
}

}  // namespace hyped::navigation