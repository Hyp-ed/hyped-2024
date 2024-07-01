#include "navigator.hpp"

#include "core/mqtt.hpp"
#include "core/types.hpp"
#include "core/wall_clock.hpp"
#include "navigation/filtering/kalman_matrices.hpp"
#include "preprocessing/preprocess_optical.hpp"

namespace hyped::navigation {

Navigator::Navigator(core::ILogger &logger,
                     const core::ITimeSource &time,
                     std::shared_ptr<core::IMqtt> mqtt)
    : logger_(logger),
      time_(time),
      mqtt_(std::move(mqtt)),
      keyence_preprocessor_(logger),
      accelerometer_preprocessor_(logger, time),
      previous_accelerometer_data_(0.0),
      previous_optical_reading_(0.0),
      previous_keyence_reading_(0.0),
      kalman_filter_(initial_state,
                     initial_covariance,
                     kStateTransitionMatrix,
                     kControlMatrix,
                     kErrorCovarianceMatrix,
                     measurement_matrix,
                     kMeasurementNoiseCovarianceMatrix)
{
}

std::optional<core::Trajectory> Navigator::currentTrajectory()
{
  control_input_vector << previous_accelerometer_data_;
  measurement_vector << previous_keyence_reading_, previous_optical_reading_;

  // Modify measurement matrix depending on the availabiiity of keyence data
  if (previous_keyence_reading_ == 0.0) {
    measurement_matrix(0, 0) = 0;
  } else {
    measurement_matrix(0, 0) = 1;
  }

  kalman_filter_.filter(measurement_vector, control_input_vector);
  previous_keyence_reading_ = 0.0;  // Reset keyence reading to 0.0 after use so that next step uses
                                    // optical measurement matrix only.

  trajectory_.displacement = kalman_filter_.getStateEstimate()(0);
  trajectory_.velocity     = kalman_filter_.getStateEstimate()(1);

  logger_.log(core::LogLevel::kInfo,
              "Trajectory successfully updated to: %f, %f",
              trajectory_.displacement,
              trajectory_.velocity);

  // TODOLater: check this
  if (trajectory_.displacement
      > static_cast<core::Float>(kTrackLength - (1.5 * kBrakingDistance))) {
    logger_.log(core::LogLevel::kFatal, "Time to brake!");
    return std::nullopt;
  }

  return trajectory_;
}

core::Result Navigator::keyenceUpdate(const core::KeyenceData &keyence_data)
{
  // Check keyence strictly increasing
  if (keyence_data.at(0) < previous_keyence_reading_) {
    logger_.log(core::LogLevel::kFatal, "Keyence data is decreasing");
    return core::Result::kFailure;
  }

  // Run preprocessing on keyence and check result
  const SensorChecks keyence_check = keyence_preprocessor_.checkKeyenceAgrees(keyence_data);
  if (keyence_check == SensorChecks::kUnacceptable) {
    logger_.log(core::LogLevel::kFatal, "Keyence data has failed preprocessing");
    return core::Result::kFailure;
  }

  previous_keyence_reading_ = keyence_data.at(0);

  logger_.log(core::LogLevel::kDebug, "Keyence data successfully updated");
  return core::Result::kSuccess;
}

core::Result Navigator::opticalUpdate(const core::OpticalData &optical_data)
{
  // Run preprocessing on optical
  const auto clean_optical_data = OpticalPreprocessor::processData(optical_data);

  // get mean value
  previous_optical_reading_ = 0.0;

  for (std::size_t i = 0; i < core::kNumOptical; ++i) {
    previous_optical_reading_ += clean_optical_data.value().at(i);
  }
  previous_optical_reading_ /= core::kNumOptical;

  logger_.log(core::LogLevel::kDebug, "Optical flow data successfully updated");
  return core::Result::kSuccess;
}

core::Result Navigator::accelerometerUpdate(const core::RawAccelerometerData &accelerometer_data)
{
  // run preprocessing, get filtered acceleration data
  auto clean_accelerometer_data = accelerometer_preprocessor_.processData(accelerometer_data);
  if (!clean_accelerometer_data) {
    logger_.log(core::LogLevel::kFatal, "Reliability error in accelerometer data");
    return core::Result::kFailure;
  }

  // get mean value
  previous_accelerometer_data_ = 0;
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    previous_accelerometer_data_ += clean_accelerometer_data.value().at(i);
  }
  previous_accelerometer_data_ /= core::kNumAccelerometers;

  logger_.log(core::LogLevel::kDebug, "Accelerometer data successfully updated");
  return core::Result::kSuccess;
}

// Publish failure message to kState topic
void Navigator::requestFailure()
{
  std::string failure_message = "kFailure";
  const auto topic            = core::MqttTopic::kState;
  auto message_payload        = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();

  rapidjson::Value message_value;
  message_value.SetString(failure_message.c_str(), message_payload->GetAllocator());
  message_payload->AddMember("transition", message_value, message_payload->GetAllocator());

  const core::MqttMessage::Header header{.timestamp = 0,
                                         .priority  = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
}

// Publish current trajectory to kTest topic
void Navigator::publishCurrentTrajectory()
{
  const auto topic     = core::MqttTopic::kNavigationData;
  auto message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();

  const auto trajectory = currentTrajectory();
  if (!trajectory) { logger_.log(core::LogLevel::kFatal, "Failed to get current trajectory"); }

  rapidjson::Value displacement(trajectory->displacement);
  rapidjson::Value velocity(trajectory->velocity);
  rapidjson::Value acceleration(previous_accelerometer_data_);
  message_payload->AddMember("displacement", displacement, message_payload->GetAllocator());
  message_payload->AddMember("velocity", velocity, message_payload->GetAllocator());
  message_payload->AddMember("acceleration", acceleration, message_payload->GetAllocator());

  const core::MqttMessage::Header header{.timestamp = 0,
                                         .priority  = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
}

void Navigator::publishStart()
{
  const auto topic     = core::MqttTopic::kStarted;
  auto message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();

  rapidjson::Value message_value;
  message_value.SetString("Nav module running", message_payload->GetAllocator());
  message_payload->AddMember("message", message_value, message_payload->GetAllocator());

  const core::MqttMessage::Header header{.timestamp = 0,
                                         .priority  = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
}

core::Result Navigator::subscribeToTopics()
{
  for (const auto &topic : kSubscribedTopics) {
    const auto result = mqtt_->subscribe(topic);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal,
                  "Failed to subscribe to topic: %s",
                  core::mqtt_topic_to_string.find(topic));
      return core::Result::kFailure;
    }
  }
  return core::Result::kSuccess;
}

void Navigator::updateSensorData(core::KeyenceData &keyence_data,
                                 core::OpticalData &optical_data,
                                 core::RawAccelerometerData &accelerometer_data)
{
  auto keyence_update_result = keyenceUpdate(keyence_data);
  if (keyence_update_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to update Keyence sensor data");
    requestFailure();
    return;
  }
  auto optical_update_result = opticalUpdate(optical_data);
  if (optical_update_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to update Optical sensor data");
    requestFailure();
    return;
  }
  auto accelerometer_update_result = accelerometerUpdate(accelerometer_data);
  if (accelerometer_update_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to update Accelerometer sensor data");
    requestFailure();
    return;
  }
}

void Navigator::run()
{
  publishStart();
  core::Result result = subscribeToTopics();
  if (result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Failed to subscribe to topics");
    requestFailure();
    return;
  }

  // TODOLater: all these are assuming there is only one sensor each
  std::optional<core::KeyenceData> most_recent_keyence_data                = std::nullopt;
  std::optional<core::OpticalData> most_recent_optical_data                = std::nullopt;
  std::optional<core::RawAccelerometerData> most_recent_accelerometer_data = {std::nullopt};

  while (true) {
    most_recent_keyence_data       = std::nullopt;
    most_recent_optical_data       = std::nullopt;
    most_recent_accelerometer_data = std::nullopt;

    while (
      !(most_recent_keyence_data && most_recent_optical_data && most_recent_accelerometer_data)) {
      const auto consume_result = mqtt_->consume();
      if (consume_result == core::Result::kFailure) {
        logger_.log(core::LogLevel::kFatal, "Invalid message received from MQTT");
      }
      auto optional_message = mqtt_->getMessage();
      if (!optional_message) { continue; }
      auto message = *optional_message;

      // TODOLater: make the parsing more robust
      switch (message.topic) {
        case core::MqttTopic::kKeyence: {
          auto payload             = message.payload;
          const auto keyence_data  = core::KeyenceData{(*payload)["keyence_data"].GetUint()};
          most_recent_keyence_data = core::KeyenceData{keyence_data};
          break;
        }
        case core::MqttTopic::kOpticalFlow: {
          auto payload             = message.payload;
          const auto x             = (*payload)["x"].GetFloat();
          const auto y             = (*payload)["y"].GetFloat();
          const auto optical_data  = core::OpticalData{{x, y}};
          most_recent_optical_data = core::OpticalData{optical_data};
          break;
        }
        case core::MqttTopic::kAccelerometer: {
          auto payload                   = message.payload;
          const auto x                   = (*payload)["x"].GetFloat();
          const auto y                   = (*payload)["y"].GetFloat();
          const auto z                   = (*payload)["z"].GetFloat();
          most_recent_accelerometer_data = core::RawAccelerometerData{{x, y, z}};
          break;
        }
        default:
          break;
      }
    }

    updateSensorData(
      *most_recent_keyence_data, *most_recent_optical_data, *most_recent_accelerometer_data);
    publishCurrentTrajectory();
  }
}

core::Result Navigator::startNode(toml::v3::node_view<const toml::v3::node> config,
                                  const std::string &mqtt_ip,
                                  const std::uint32_t mqtt_port)
{
  auto time          = core::WallClock();
  auto logger        = core::Logger("Navigation", core::LogLevel::kDebug, time);
  auto optional_mqtt = core::Mqtt::create(logger, "Navigation", mqtt_ip, mqtt_port);
  if (!optional_mqtt) {
    logger.log(core::LogLevel::kFatal, "Failed to create MQTT client");
    return core::Result::kFailure;
  }
  auto mqtt = *optional_mqtt;
  Navigator navigator(logger, time, mqtt);
  navigator.run();
  return core::Result::kSuccess;
}

}  // namespace hyped::navigation
