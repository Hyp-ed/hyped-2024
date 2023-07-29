#pragma once

#include <cstdint>
#include <vector>

#include <core/logger.hpp>
#include <core/time.hpp>
#include <core/timer.hpp>
#include <core/types.hpp>
#include <navigation/control/consts.hpp>

namespace hyped::utils {
class KalmanHigherDerivatives {
 public:
  KalmanHigherDerivatives(core::ILogger &logger, const core::ITimeSource &time);

  navigation::HigherDerivatives getHigherDerivatives(const std::uint64_t timestamp,
                                                     const core::Float acceleration);

 private:
  core::ILogger &logger_;
  const core::ITimeSource &time_;

  // TODOLater: decide 50 or 100 - numerical tests?
  static constexpr std::uint8_t kNumDataPoints = 50;

  std::vector<std::uint64_t> timestamps_;
  std::vector<core::Float> acceleration_values_;
  std::vector<core::Float> jerk_values_;
  std::vector<core::Float> snap_values_;

  core::Float sum_jerk_;
  core::Float sum_snap_;
};
}  // namespace hyped::utils