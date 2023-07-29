#pragma once

#include <cstdint>
#include <optional>

#include <core/time.hpp>
#include <core/timer.hpp>
#include <core/types.hpp>

namespace hyped::navigation {

class AccelerometerTrajectoryEstimator {
 public:
  AccelerometerTrajectoryEstimator(const core::ITimeSource &time);
  /**
   * @brief update the imu estimate for both displacement and velocity. Displacement is
   * used for cross checking agreement with wheel encooders, velocity is used for us to
   * estimate velocity in the case we have no direct velocity sensors.
   *
   * @param imu_acceleration kalman filtered single value for acceleration
   * @param imu_timestamp time at which measurement was taken
   */
  void update(const core::Float imu_acceleration, const core::TimePoint imu_timestamp);

  core::Float getDisplacementEstimate() const;

  core::Float getVelocityEstimate() const;

 private:
  core::Float displacement_estimate_;
  core::Float velocity_estimate_;
  core::TimePoint previous_timestamp_;
  const core::ITimeSource &time_;
};

}  // namespace hyped::navigation