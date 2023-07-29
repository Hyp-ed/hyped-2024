#pragma once

#include <cstdint>
#include <vector>

#include <core/logger.hpp>
#include <core/time.hpp>
#include <core/types.hpp>
#include <navigation/control/consts.hpp>

namespace hyped::navigation {

class RunningMeansFilter {
 public:
  RunningMeansFilter(core::ILogger &logger, const core::ITimeSource &time);

  core::Float updateEstimate(const core::Float new_acceleration_value);

 private:
  static constexpr std::uint8_t kNumDataPoints = 20;
  core::ILogger &logger_;
  const core::ITimeSource &time_;
  std::vector<core::Float> acceleration_values_;
};
}  // namespace hyped::navigation