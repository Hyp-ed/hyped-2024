#include "running_means_filter.hpp"

namespace hyped::navigation {

RunningMeansFilter::RunningMeansFilter(core::ILogger &logger, const core::ITimeSource &time)
    : logger_(logger),
      time_(time)
{
  acceleration_values_.resize(kNumDataPoints);
  std::fill(acceleration_values_.begin(), acceleration_values_.end(), 0.0F);
}

core::Float RunningMeansFilter::updateEstimate(const core::Float new_acceleration_value)
{
  acceleration_values_.pop_back();
  acceleration_values_.insert(acceleration_values_.begin(), new_acceleration_value);

  core::Float sum_acceleration_values = 0;
  for (std::size_t i = 0; i < acceleration_values_.size(); ++i) {
    sum_acceleration_values += acceleration_values_.at(i);
  }

  return sum_acceleration_values / acceleration_values_.size();
}
}  // namespace hyped::navigation