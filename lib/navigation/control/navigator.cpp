#include "navigator.hpp"

#include <cstdint>

namespace hyped::navigation {
// TODOLater: check we stop once near calculated safe stopping distance

Navigator::Navigator(core::ILogger &logger, const core::ITimeSource &time)
    : logger_(logger),
      time_(time),
      keyence_preprocessor_(logger),
      accelerometer_preprocessor_(logger, time),
      accelerometer_trajectory_estimator_(time),
      // crosschecker_(logger, time),
      running_means_filter_(logger, time),
      encoders_preprocessor_(logger),
      previous_keyence_reading_({0, 0})
{
}

// TODOLater: make sure that whoever calls this calls a full fail state if return fo this function
// is std::nullopt
std::optional<core::Trajectory> Navigator::currentTrajectory()
{
  // TODOLater: uncomment when wheel encoders working
  // get mean values from arrays to use in crosschecking
  /*
  core::Float mean_encoder_value = 0;
  for (std::size_t i = 0; i < core::kNumEncoders; ++i) {
    mean_encoder_value += static_cast<core::Float>(previous_encoder_reading_.at(i));
  }
  mean_encoder_value /= core::kNumEncoders;
  */

  core::Float mean_keyence_value = 0;
  for (std::size_t i = 0; i < core::kNumKeyence; ++i) {
    mean_keyence_value += static_cast<core::Float>(previous_keyence_reading_.at(i));
  }
  mean_keyence_value /= core::kNumKeyence;

  // TODOLater: use again when wheel encoders work
  // cross check all estimates to ensure any returned trajectory is accurate
  /*
  const SensorChecks check_trajectory = crosschecker_.checkTrajectoryAgreement(
    trajectory_.displacement, mean_encoder_value, mean_keyence_value);
  */
  // temp solution
  SensorChecks check_trajectory = SensorChecks::kAcceptable;
  if (std::abs(trajectory_.displacement - mean_keyence_value) > 10) {
    check_trajectory = SensorChecks::kUnacceptable;
  }

  // check fail state
  if (check_trajectory == SensorChecks::kUnacceptable) {
    logger_.log(core::LogLevel::kFatal,
                "Navigation sensors are in disagreement. Unable to accurately determine "
                "trajectory.");
    return std::nullopt;
  }

  // TODOLater: check braking implementation here!
  if (trajectory_.displacement
      > static_cast<core::Float>(kTrackLength - (1.5 * kBrakingDistance))) {
    logger_.log(core::LogLevel::kFatal, "Time to break!");
    return std::nullopt;
  }

  return trajectory_;
}

// TODOLater: check input from sensors matches this
core::Result Navigator::keyenceUpdate(const core::KeyenceData &keyence_data)
{
  // Check keyence strictly increasing
  if (keyence_data.at(0) < previous_keyence_reading_.at(0)) {
    logger_.log(core::LogLevel::kFatal, "Keyence data is decreasing");
    return core::Result::kFailure;
  }

  // Run preprocessing on keyence and check result
  const SensorChecks keyence_check = keyence_preprocessor_.checkKeyenceAgrees(keyence_data);
  if (keyence_check == SensorChecks::kUnacceptable) {
    logger_.log(core::LogLevel::kFatal, "Keyence data has failed preprocessing");
    return core::Result::kFailure;
  }

  // Update old keyence reading
  previous_keyence_reading_ = keyence_data;
  logger_.log(core::LogLevel::kInfo, "Keyence data successfully updated in Navigation");
  return core::Result::kSuccess;
}

// TODOLater: check input from sensors matches this
// THIS SHOULD NOT BE CALLED!
core::Result Navigator::encoderUpdate(const core::EncoderData &encoder_data)
{
  // check encoder data strictly increasing
  for (std::size_t i = 0; i < core::kNumEncoders; ++i) {
    if (previous_encoder_reading_.at(i) < encoder_data.at(i)) {
      logger_.log(core::LogLevel::kFatal, "Encoder data is decreasing");
      return core::Result::kFailure;
    }
  }

  // run preprocessing on encoder data
  auto clean_encoder_data = encoders_preprocessor_.processData(encoder_data);

  // check fail state
  if (!clean_encoder_data) {
    logger_.log(core::LogLevel::kFatal, "Encoder data has failed preprocessing");
    return core::Result::kFailure;
  }

  // update internal value
  previous_encoder_reading_ = clean_encoder_data.value();
  logger_.log(core::LogLevel::kInfo, "Encoder data successfully updated in navigation");
  return core::Result::kSuccess;
}

// TODOLater: check input from sensors matches this
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
  core::Float unfiltered_acceleration = 0;
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    unfiltered_acceleration += clean_accelerometer_data.value().at(i);
  }
  unfiltered_acceleration /= core::kNumAccelerometers;

  // run filtering on estimate
  // TODOLater: change from rolling means to kalamn
  const core::Float filtered_acceleration
    = running_means_filter_.updateEstimate(unfiltered_acceleration);

  // Numerically integrate data estimates, update internal class values
  accelerometer_trajectory_estimator_.update(filtered_acceleration,
                                             accelerometer_data.at(0).measured_at);
  trajectory_.acceleration = filtered_acceleration;
  trajectory_.velocity     = accelerometer_trajectory_estimator_.getVelocityEstimate();
  trajectory_.displacement = accelerometer_trajectory_estimator_.getDisplacementEstimate();

  // check if we need to break
  if (trajectory_.displacement
      > static_cast<core::Float>(kTrackLength - (1.5 * kBrakingDistance))) {
    logger_.log(core::LogLevel::kFatal, "Time to break!");
    return core::Result::kFailure;
  }

  logger_.log(core::LogLevel::kInfo, "Navigation trjectory successfully updated.");
  return core::Result::kSuccess;
}

}  // namespace hyped::navigation
