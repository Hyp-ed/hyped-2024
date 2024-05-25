#include "navigator.hpp"

#include <cstdint>

namespace hyped::navigation {
// TODOLater: check we stop once near calculated safe stopping distance

Navigator::Navigator(core::ILogger &logger,
                     const core::ITimeSource &time,
                     std::shared_ptr<core::IMqtt> mqtt)
    : logger_(logger),
      time_(time),
      mqtt_(std::move(mqtt)),
      keyence_preprocessor_(logger),
      optical_preprocessor_(logger),
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
  // TODOLater: check fail state if required

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
                                    // optical measurment matrix only.

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

// TODOLater: check input from sensors matches this
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

  logger_.log(core::LogLevel::kInfo, "Keyence data successfully updated");
  return core::Result::kSuccess;
}
core::Result Navigator::opticalUpdate(const core::OpticalData &optical_data)
{
  // Run preprocessing on optical
  const auto clean_optical_data = optical_preprocessor_.processData(optical_data);

  // get mean value
  previous_optical_reading_ = 0.0;

  for (std::size_t i = 0; i < core::kNumOptical; ++i) {
    previous_optical_reading_ += clean_optical_data.value().at(i);
  }
  previous_optical_reading_ /= core::kNumOptical;

  logger_.log(core::LogLevel::kInfo, "Optical flow data successfully updated");
  return core::Result::kSuccess;
}

core::Result Navigator::accelerometerUpdate(
  const std::array<core::RawAccelerationData, core::kNumAccelerometers> &accelerometer_data)
{
  // reformat raw data
  core::RawAccelerometerData reformatted_data;
  std::array<core::Float, core::kNumAxis> temp_array;
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    temp_array.at(0)       = accelerometer_data.at(i).x;
    temp_array.at(1)       = accelerometer_data.at(i).y;
    temp_array.at(2)       = accelerometer_data.at(i).z;
    reformatted_data.at(i) = temp_array;
  }

  // run preprocessing, get filtered acceleration data
  auto clean_accelerometer_data = accelerometer_preprocessor_.processData(reformatted_data);
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

  logger_.log(core::LogLevel::kInfo, "Accelerometer data successfully updated");
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
  const auto topic     = core::MqttTopic::kTest;
  auto message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();

  auto trajectory = currentTrajectory();
  if (!trajectory) { logger_.log(core::LogLevel::kFatal, "Failed to get current trajectory"); }

  rapidjson::Value displacement(trajectory->displacement);
  rapidjson::Value velocity(trajectory->velocity);
  message_payload->AddMember("displacement", displacement, message_payload->GetAllocator());
  message_payload->AddMember("velocity", velocity, message_payload->GetAllocator());

  const core::MqttMessage::Header header{.timestamp = 0,
                                         .priority  = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
}

bool Navigator::subscribeAndCheck(core::MqttTopic topic)
{
  auto result = mqtt_->subscribe(topic);
  if (result == core::Result::kFailure) {
    requestFailure();
    logger_.log(core::LogLevel::kFatal, "Failed to subscribe to topic");
    return false;
  }
  return true;
}

void Navigator::run()
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

  // Subscribe to all required topics

  if (!subscribeAndCheck(core::MqttTopic::kKeyence)
      || !subscribeAndCheck(core::MqttTopic::kOpticalFlow)
      || !subscribeAndCheck(core::MqttTopic::kAccelerometer)) {
    return;
  }

  while (true) {
    mqtt_->consume();

    auto keyence_msg       = mqtt_->getMessage();
    auto optical_msg       = mqtt_->getMessage();
    auto accelerometer_msg = mqtt_->getMessage();

    auto keyence_payload          = keyence_msg->payload;
    auto optical_payload          = optical_msg->payload;
    auto raw_acceleration_payload = accelerometer_msg->payload;

    core::KeyenceData dummy_keyence_data = {0, 0};

    core::OpticalData dummy_optical_data;
    for (auto &data : dummy_optical_data) {
      data = {0.0F, 0.0F};
    }

    std::array<core::RawAccelerationData, core::kNumAccelerometers> dummy_accelerometer_data
      = {core::RawAccelerationData(0, 0, 0, core::TimePoint{}, false),
         core::RawAccelerationData(0, 0, 0, core::TimePoint{}, false),
         core::RawAccelerationData(0, 0, 0, core::TimePoint{}, false),
         core::RawAccelerationData(0, 0, 0, core::TimePoint{}, false)};

    auto keyence_update_result       = keyenceUpdate(dummy_keyence_data);
    auto optical_update_result       = opticalUpdate(dummy_optical_data);
    auto accelerometer_update_result = accelerometerUpdate(dummy_accelerometer_data);

    if (keyence_update_result == core::Result::kFailure
        || optical_update_result == core::Result::kFailure
        || accelerometer_update_result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to update sensor data");
      requestFailure();

      break;
    }

    publishCurrentTrajectory();
  }
}

}  // namespace hyped::navigation