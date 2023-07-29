#include "preprocess_accelerometer.hpp"

namespace hyped::navigation {

AccelerometerPreprocessor::AccelerometerPreprocessor(core::ILogger &logger,
                                                     const core::ITimeSource &time)
    : logger_(logger),
      time_(time),
      num_outliers_per_accelerometer_({0, 0, 0, 0}),
      are_accelerometers_reliable_({true, true, true, true}),
      num_reliable_accelerometers_(core::kNumAccelerometers)
{
}

std::optional<core::AccelerometerData> AccelerometerPreprocessor::processData(
  const core::RawAccelerometerData raw_accelerometer_data)
{
  core::AccelerometerData accelerometer_data;
  // TODOLater : check on direction of travel
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    core::Float magnitude = 0;
    for (std::size_t j = 0; j < core::kNumAxis; ++j) {
      magnitude += std::pow(raw_accelerometer_data.at(i).at(j), 2);
    }
    accelerometer_data.at(i) = std::sqrt(magnitude);
  }
  const core::AccelerometerData clean_accelerometer_data = handleOutliers(accelerometer_data);
  SensorChecks sensorcheck                               = checkReliable();

  if (sensorcheck == SensorChecks::kUnacceptable) { return std::nullopt; }
  return clean_accelerometer_data;
}

core::AccelerometerData AccelerometerPreprocessor::handleOutliers(
  core::AccelerometerData accelerometer_data)
{
  // core::AccelerometerData accelerometer_data;
  // for (std::size_t i; i < core::kNumAccelerometers; ++i) {
  //   accelerometer_data.at(i) = accel_data.at(i);
  // }
  Quartiles quartiles;
  if (num_reliable_accelerometers_ == core::kNumAccelerometers) {
    quartiles = getQuartiles(accelerometer_data);
  } else if (num_reliable_accelerometers_ == core::kNumAccelerometers - 1) {
    std::array<core::Float, core::kNumAccelerometers - 1> filtered_data;
    std::size_t j = 0;
    for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
      if (are_accelerometers_reliable_.at(i)) {
        filtered_data.at(j) = accelerometer_data.at(i);
        ++j;
      }
    }
    quartiles = getQuartiles(filtered_data);
  } else {
    logger_.log(core::LogLevel::kFatal, "Maximum number of unreliable accelerometers exceeded");
  }
  // TODOLater : maybe make a nice data structure
  const core::Float iqr = quartiles.q3 - quartiles.q1;
  core::Float lower_bound;
  core::Float upper_bound;
  // TODOLater : Check these values
  if (num_reliable_accelerometers_ == core::kNumAccelerometers) {
    lower_bound = quartiles.median - 1.5 * iqr;
    upper_bound = quartiles.median + 1.5 * iqr;
  } else {
    lower_bound = quartiles.median - 1.2 * iqr;
    upper_bound = quartiles.median + 1.2 * iqr;
  }
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    // converts outliers or unreliables to medians, updates number of consecutive outliers for each
    // sensor
    if (are_accelerometers_reliable_.at(i) == false) {
      accelerometer_data.at(i) = quartiles.median;
    } else if (accelerometer_data.at(i) < lower_bound || accelerometer_data.at(i) > upper_bound) {
      accelerometer_data.at(i) = quartiles.median;
      ++num_outliers_per_accelerometer_.at(i);
    } else {
      num_outliers_per_accelerometer_.at(i) = 0;
    }
  }
  return accelerometer_data;
}

SensorChecks AccelerometerPreprocessor::checkReliable()
{  // changes reliable sensor to false if max consecutive outliers are reached
  for (std::size_t i = 0; i < core::kNumAccelerometers; ++i) {
    if (are_accelerometers_reliable_.at(i) == true
        && num_outliers_per_accelerometer_.at(i) >= kNumAllowedAccelerometerFailures_) {
      are_accelerometers_reliable_.at(i) = false;
      num_reliable_accelerometers_ -= 1;
    }
  }
  if (num_reliable_accelerometers_ < core::kNumAccelerometers - 1) {
    logger_.log(core::LogLevel::kFatal, "Maximum number of unreliable accelerometers exceeded");
    return SensorChecks::kUnacceptable;
  }
  return SensorChecks::kAcceptable;
}

}  // namespace hyped::navigation