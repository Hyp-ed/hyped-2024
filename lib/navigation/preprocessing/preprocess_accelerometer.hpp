#pragma once

#include <cmath>

#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>

#include <core/logger.hpp>
#include <core/time.hpp>
#include <core/timer.hpp>
#include <core/types.hpp>
#include <navigation/control/consts.hpp>

#if defined(__linux__)
#include <eigen3/Eigen/Dense>
//#else
// TODO: add appropriate eigen include for mac here
#endif

namespace hyped::navigation {
class AccelerometerPreprocessor {
 public:
  AccelerometerPreprocessor(core::ILogger &logger, const core::ITimeSource &time);

  /**
   * @brief convert raw accelerometer data to cleaned and filtered data
   *
   * @param raw_accelerometer_data
   * @return clean accelerometer data or optionally fail
   */
  std::optional<core::AccelerometerData> processData(
    const core::RawAccelerometerData raw_accelerometer_data);

 private:
  core::ILogger &logger_;
  const core::ITimeSource &time_;

  std::array<std::uint16_t, core::kNumAccelerometers> num_outliers_per_accelerometer_;
  std::array<bool, core::kNumAccelerometers> are_accelerometers_reliable_;
  std::size_t num_reliable_accelerometers_;
  // TODOLater: implement this (maybe const and on construction?)
  Eigen::Matrix<core::Float, 1, 3> measurement_matrix_;

  // number of allowed consecutive outliers from single accelerometer
  static constexpr std::uint8_t kNumAllowedAccelerometerFailures_ = 20;

  /**
   * @brief filter the accelerometer data by converting outliers to median value
   *
   * @param accelerometer_data
   * @return filtered accelerometer data
   */
  core::AccelerometerData handleOutliers(core::AccelerometerData accelerometer_data);

  /**
   * @brief check the reliability of all accelerometer's
   *
   * @return SensorChecks with value kAcceptable or kUnacceptable
   */
  SensorChecks checkReliable();

  // TODOLater: Optimize this further
  /**
   * @brief find value at a specific quantile of the data
   *
   * @param clean_accelerometer_data
   * @param quartile
   * @return value at specified quantile
   */
  template<std::size_t N>
  core::Float getSpecificQuantile(const std::array<core::Float, N> &clean_accelerometer_data,
                                  core::Float quartile)
  {
    const core::Float index_quartile = (num_reliable_accelerometers_ - 1) * quartile;
    const int index_quartile_high    = static_cast<int>(std::ceil(index_quartile));
    const int index_quartile_low     = static_cast<int>(std::floor(index_quartile));
    const core::Float quartile_value = (clean_accelerometer_data.at(index_quartile_high)
                                        + clean_accelerometer_data.at(index_quartile_low))
                                       / 2.0;
    return quartile_value;
  }

  /**
   * @brief find values at each quartile of the data
   *
   * @param accelerometer_data
   * @return values of each quartile
   */
  template<std::size_t N>
  Quartiles getQuartiles(const std::array<core::Float, N> &accelerometer_data)
  {
    std::array<core::Float, N> accelerometer_data_copy;
    std::copy(
      accelerometer_data.begin(), accelerometer_data.end(), accelerometer_data_copy.begin());

    std::sort(accelerometer_data_copy.begin(), accelerometer_data_copy.end());
    return {.q1     = getSpecificQuantile(accelerometer_data_copy, 0.25),
            .median = getSpecificQuantile(accelerometer_data_copy, 0.5),
            .q3     = getSpecificQuantile(accelerometer_data_copy, 0.75)};
  }
};

}  // namespace hyped::navigation