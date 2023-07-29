#pragma once

#include "consts.hpp"
#include "crosscheck.hpp"

#include <array>
#include <memory>
#include <optional>

#include "core/logger.hpp"
#include "core/types.hpp"
#include "navigation/control/consts.hpp"
#include "navigation/filtering/running_means_filter.hpp"
#include "navigation/preprocessing/accelerometer_trajectory.hpp"
#include "navigation/preprocessing/preprocess_accelerometer.hpp"
#include "navigation/preprocessing/preprocess_encoders.hpp"
#include "navigation/preprocessing/preprocess_keyence.hpp"

namespace hyped::navigation {

class Navigator : public INavigator {
 public:
  Navigator(core::ILogger &logger, const core::ITimeSource &time);

  /**
   *@brief runs cross checking and returns trajectory
   */
  std::optional<core::Trajectory> currentTrajectory();

  /**
   * @brief preprocesses keyence data and updates trajectory
   *
   * @param keyence_data
   */
  core::Result keyenceUpdate(const core::KeyenceData &keyence_data);
  /**
   * @brief preprocesses encoder data and updates trajectory
   *
   * @param encoder_data
   */
  core::Result encoderUpdate(const core::EncoderData &encoder_data);
  /**
   * @brief preprocesses accelerometer data and updates trajectory
   *
   * @param accelerometer_data
   */
  core::Result accelerometerUpdate(
    const std::array<core::RawAccelerationData, core::kNumAccelerometers> &accelerometer_data);

 private:
  core::ILogger &logger_;
  const core::ITimeSource &time_;

  // navigation functionality
  KeyencePreprocessor keyence_preprocessor_;
  AccelerometerPreprocessor accelerometer_preprocessor_;
  EncodersPreprocessor encoders_preprocessor_;
  AccelerometerTrajectoryEstimator accelerometer_trajectory_estimator_;
  // TODOLater: use again when wheel encoders work
  // Crosscheck crosschecker_;
  RunningMeansFilter running_means_filter_;

  // previous readings
  core::EncoderData previous_encoder_reading_;
  core::KeyenceData previous_keyence_reading_;

  // current navigation trajectory
  core::Trajectory trajectory_;
};

}  // namespace hyped::navigation