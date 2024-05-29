#pragma once

#include "consts.hpp"

#include <array>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/mqtt.hpp>
#include <core/types.hpp>
#include <navigation/filtering/kalman_filter.hpp>
#include <navigation/filtering/kalman_matrices.hpp>
#include <navigation/preprocessing/preprocess_accelerometer.hpp>
#include <navigation/preprocessing/preprocess_keyence.hpp>
#include <navigation/preprocessing/preprocess_optical.hpp>

namespace hyped::navigation {

class Navigator : public INavigator {
 public:
  Navigator(core::ILogger &logger,
            const core::ITimeSource &time,
            std::shared_ptr<core::IMqtt> mqtt);
  void run();
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
   * @brief Preprocesses optical flow data and updates trajectory
   *
   * @param optical_data
   */
  core::Result opticalUpdate(const core::OpticalData &optical_data);
  /**
   * @brief preprocesses accelerometer data and updates trajectory
   *
   * @param accelerometer_data
   */
  core::Result accelerometerUpdate(
    const std::array<core::RawAccelerationData, core::kNumAccelerometers> &accelerometer_data);

  /**
   * @brief Publishes the current trajectory to the MQTT broker
   */
  void publishCurrentTrajectory();

  /**
   * @brief Publishes a failure message to the MQTT broker
   */
  void requestFailure();

  /**
   * @brief Checks if subscribing to a topic was successful
   *
   * @param message
   */
  bool subscribeAndCheck(core::MqttTopic topic);

 private:
  core::ILogger &logger_;
  const core::ITimeSource &time_;

  std::shared_ptr<core::IMqtt> mqtt_;

  KalmanFilter kalman_filter_;

  // navigation functionality
  KeyencePreprocessor keyence_preprocessor_;
  AccelerometerPreprocessor accelerometer_preprocessor_;
  OpticalPreprocessor optical_preprocessor_;

  // previous readings
  core::Float previous_optical_reading_;
  core::Float previous_keyence_reading_;
  core::Float previous_accelerometer_data_;

  core::Result keyence_update_result       = core::Result::kSuccess;
  core::Result optical_update_result       = core::Result::kSuccess;
  core::Result accelerometer_update_result = core::Result::kSuccess;

  // current navigation trajectory
  core::Trajectory trajectory_;
};

}  // namespace hyped::navigation