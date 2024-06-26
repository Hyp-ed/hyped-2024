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
#include <toml++/toml.hpp>

namespace hyped::navigation {

class Navigator : public INavigator {
 public:
  Navigator(core::ILogger &logger,
            const core::ITimeSource &time,
            std::shared_ptr<core::IMqtt> mqtt);

  // Run the navigator as an mqtt node
  static core::Result startNode(toml::v3::node_view<const toml::v3::node> config,
                                const std::string &mqtt_ip,
                                const std::uint32_t mqtt_port);

  /**
   *@brief runs cross checking and returns trajectory
   */
  std::optional<core::Trajectory> currentTrajectory() override;

  /**
   * @brief preprocesses keyence data and updates trajectory
   *
   * @param keyence_data
   */
  core::Result keyenceUpdate(const core::KeyenceData &keyence_data) override;
  /**
   * @brief Preprocesses optical flow data and updates trajectory
   *
   * @param optical_data
   */
  core::Result opticalUpdate(const core::OpticalData &optical_data) override;
  /**
   * @brief preprocesses accelerometer data and updates trajectory
   *
   * @param accelerometer_data
   */
  core::Result accelerometerUpdate(const core::RawAccelerometerData &accelerometer_data) override;

 private:
  void run();
  /**
   * @brief Publishes the current trajectory to the MQTT broker
   */
  core::Result publishCurrentTrajectory();

  /**
   * @brief Publishes a failure message to the MQTT broker
   */
  void requestFailure();

  /**
   * @brief Publishes a start message to the MQTT broker
   */
  void publishStart();

  /**
   * @brief Publishes a failure message to the MQTT broker
   */
  void requestBraking();

  /**
   * @brief Subscribes to topics
   */
  core::Result subscribeToTopics();

  /**
   * @brief Updates the sensor data and the trajectory
   *
   * @param keyence_data
   * @param optical_data
   * @param accelerometer_data
   */
  void updateSensorData(core::KeyenceData &keyence_data,
                        core::OpticalData &optical_data,
                        core::RawAccelerometerData &accelerometer_data);

  core::ILogger &logger_;
  const core::ITimeSource &time_;

  std::shared_ptr<core::IMqtt> mqtt_;

  KalmanFilter kalman_filter_;

  // navigation functionality
  KeyencePreprocessor keyence_preprocessor_;
  AccelerometerPreprocessor accelerometer_preprocessor_;

  // previous readings
  core::Float previous_optical_reading_;
  core::Float previous_keyence_reading_;
  core::Float previous_accelerometer_data_;

  core::Result keyence_update_result       = core::Result::kSuccess;
  core::Result optical_update_result       = core::Result::kSuccess;
  core::Result accelerometer_update_result = core::Result::kSuccess;

  // current navigation trajectory
  core::Trajectory trajectory_;

  constexpr static std::array<core::MqttTopic, 3> kSubscribedTopics
    = {core::MqttTopic::kKeyence, core::MqttTopic::kOpticalFlow, core::MqttTopic::kAccelerometer};
};

}  // namespace hyped::navigation
