#include "kalman_higher_derivatives.hpp"

#include <algorithm>

namespace hyped::utils {
KalmanHigherDerivatives::KalmanHigherDerivatives(core::ILogger &logger,
                                                 const core::ITimeSource &time)
    : logger_(logger),
      time_(time),
      sum_jerk_(0),
      sum_snap_(0)
{
  acceleration_values_.resize(kNumDataPoints);
  std::fill(acceleration_values_.begin(), acceleration_values_.end(), 0.0F);

  timestamps_.resize(kNumDataPoints);
  std::fill(timestamps_.begin(), timestamps_.end(), 0);

  jerk_values_.resize(kNumDataPoints - 1);
  std::fill(jerk_values_.begin(), jerk_values_.end(), 0.0F);

  snap_values_.resize(kNumDataPoints - 2);
  std::fill(snap_values_.begin(), snap_values_.end(), 0.0F);
}

navigation::HigherDerivatives KalmanHigherDerivatives::getHigherDerivatives(
  const std::uint64_t timestamp, const core::Float acceleration)
{
  // update acceleration and timestamps vectors
  acceleration_values_.pop_back();
  acceleration_values_.insert(acceleration_values_.begin(), acceleration);

  timestamps_.pop_back();
  timestamps_.insert(timestamps_.begin(), timestamp);

  // calculate new jerk value from new data and update jerk sum
  sum_jerk_ -= jerk_values_.at(jerk_values_.size() + 1);
  jerk_values_.pop_back();
  const core::Float new_jerk_value = (acceleration_values_.at(1) - acceleration_values_.at(0))
                                     / (timestamps_.at(1) - timestamps_.at(0));
  jerk_values_.insert(jerk_values_.begin(), new_jerk_value);
  sum_jerk_ += new_jerk_value;

  // calculate new snap value from new data and update snap sum
  sum_snap_ -= snap_values_.at(snap_values_.size() + 1);
  snap_values_.pop_back();
  const core::Float new_snap_value
    = (jerk_values_.at(1) - jerk_values_.at(0)) / (timestamps_.at(1) - timestamps_.at(0));
  snap_values_.insert(snap_values_.begin(), new_snap_value);
  sum_snap_ += new_snap_value;

  navigation::HigherDerivatives higher_derivatives;
  higher_derivatives.jerk = sum_jerk_ / jerk_values_.size();
  higher_derivatives.snap = sum_snap_ / snap_values_.size();

  return higher_derivatives;
}

}  // namespace hyped::utils