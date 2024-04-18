#include "navigator.hpp"

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

  previous_keyence_reading_ = keyence_data[0];

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

core::KeyenceData parseKeyenceData(const std::shared_ptr<rapidjson::Document>& payload);
core::OpticalData parseOpticalData(const std::shared_ptr<rapidjson::Document>& payload);
std::array<core::RawAccelerationData, core::kNumAccelerometers> parseAccelerometerData(const std::shared_ptr<rapidjson::Document>& payload);

// TODO: main run loop

void Navigator::run()
{
  while (true) {
    const auto keyenceMessage       = mqtt_->getMessage();
    const auto opticalMessage       = mqtt_->getMessage();
    const auto accelerometerMessage = mqtt_->getMessage();

    core::KeyenceData keyence_data;
    core::OpticalData optical_data;
    std::array<core::RawAccelerationData, core::kNumAccelerometers> accelerometer_data;

    if (!keyenceMessage || !opticalMessage || !accelerometerMessage) {
      logger_.log(core::LogLevel::kFatal, "Failed to get MQTT message");
      break;
    }

    core::KeyenceData keyence_data = parseKeyenceData(keyenceMessage->payload);
    core::OpticalData optical_data = parseOpticalData(opticalMessage->payload);
    std::array<core::RawAccelerationData, core::kNumAccelerometers> accelerometer_data
      = parseAccelerometerData(accelerometerMessage->payload);

    // update sensor readings
    if (keyenceUpdate(keyence_data) == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to update Keyence data");
      break;
    }
    if (opticalUpdate(optical_data) == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to update optical data");
      break;
    }
    if (accelerometerUpdate(accelerometer_data) == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to update accelerometer data");
      break;
    }

    auto trajectory = currentTrajectory();
    if (!trajectory) {
      logger_.log(core::LogLevel::kFatal, "Failed to calculate current trajectory");
      break;
    }
  }
}

}  // namespace hyped::navigation
