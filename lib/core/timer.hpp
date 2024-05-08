#pragma once

#include "time.hpp"
#include "types.hpp"

#include <functional>

namespace hyped::core {

constexpr std::uint32_t kOneSecond = 1'000'000'000;

class Timer {
 public:
  explicit Timer(const ITimeSource &time);

  Duration measureExecutionTime(const std::function<void(void)> &task);

  static Duration elapsed(const TimePoint current_timepoint, const TimePoint previous_timepoint);

  static Float elapsedTimeInSeconds(const Duration time_elapsed);

 private:
  const ITimeSource &time_;
};

}  // namespace hyped::core
