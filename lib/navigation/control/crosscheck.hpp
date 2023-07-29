#pragma once

#include "consts.hpp"

#include <array>

#include <core/logger.hpp>
#include <core/time.hpp>
#include <core/timer.hpp>
#include <core/types.hpp>
#include <navigation/preprocessing/accelerometer_trajectory.hpp>

namespace hyped::navigation {

class Crosscheck {
 public:
  Crosscheck(core::ILogger &logger, const core::ITimeSource &time);

  /**
   * @brief checks that all sensors agree with wheel encoders. This is a safety
   * check for the reliability of the current trajectory (i.e. the sensors are
   * probably not all wrong...)
   *
   * @return true signifies trajectory agreement
   * @return false signifies trajectory disagreement. We enter fail state
   */
  SensorChecks checkTrajectoryAgreement(const core::Float acceleration,
                                        const core::Float encoder_displacement,
                                        const core::Float keyence_displacement);

 private:
  /**
   * @brief Checks the double integrated accelerometer value of displacement against
   * the encoder value of displacement to some tolerance
   * TODOLater: update comment with tolerance once updated
   *
   * @return true accelerometer and wheel encoders agree
   * @return false accelerometers and wheel encoders disagree
   */
  SensorChecks checkEncoderAccelerometer(const core::Float acceleration,
                                         const core::Float encoder_displacement);

  /**
   * @brief Checks the keyence value of displacement against the
   * encoder value of displacement to some tolerance
   * TODOLater: update comment with tolerance once updated
   *
   * @return true Keyence and wheel encoders agree
   * @return false Keyence and wheel encoders disagree
   */
  SensorChecks checkEncoderKeyence(const core::Float encoder_displacement,
                                   const core::Float keyence_displacement);

  core::ILogger &logger_;
  const core::ITimeSource &time_;
  static constexpr std::uint8_t kMaxAllowedAccelerometerEncoderDifference = 5;
  // Allow for 1 stripe error plus some error in encoder data
  static constexpr std::uint8_t kMaxAllowedKeyenceEncoderDifference = kStripeDistance + 2;
};
}  // namespace hyped::navigation