#include "navigator.hpp"

#include <cstdint>

namespace hyped::navigation {
// TODOLater: check we stop once near calculated safe stopping distance

Navigator::Navigator(core::ILogger &logger, const core::ITimeSource &time)
    : logger_(logger),
      time_(time),
      keyence_preprocessor_(logger),
      accelerometer_preprocessor_(logger, time),
      previous_accelerometer_data_(0.0),
      previous_keyence_reading_(0.0)
{
}

// TODOLater: make sure that whoever calls this calls a full fail state if return fo this function
// is std::nullopt
std::optional<core::Trajectory> Navigator::currentTrajectory()
{
  
  // // temp solution
  // SensorChecks check_trajectory = SensorChecks::kAcceptable;
  // if (std::abs(trajectory_.displacement - mean_keyence_value) > 10) {
  //   check_trajectory = SensorChecks::kUnacceptable;
  // }

  // // check fail state
  // if (check_trajectory == SensorChecks::kUnacceptable) {
  //   logger_.log(core::LogLevel::kFatal,
  //               "Navigation sensors are in disagreement. Unable to accurately determine "
  //               "trajectory.");
  //   return std::nullopt;
  // }







  // // TODOLater: check braking implementation here!
  // if (trajectory_.displacement
  //     > static_cast<core::Float>(kTrackLength - (1.5 * kBrakingDistance))) {
  //   logger_.log(core::LogLevel::kFatal, "Time to break!");
  //   return std::nullopt;
  // }

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

  logger_.log(core::LogLevel::kInfo, "Keyence data successfully updated in Navigation");
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
  previous_accelerometer_data_ = 0;
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    previous_accelerometer_data_ += clean_accelerometer_data.value().at(i);
  }

  previous_accelerometer_data_ /= core::kNumAccelerometers;

  logger_.log(core::LogLevel::kInfo, "Navigation trjectory successfully updated.");
  return core::Result::kSuccess;
}

}  // namespace hyped::navigation
