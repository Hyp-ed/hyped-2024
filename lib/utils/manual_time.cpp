
#include "manual_time.hpp"

#include <chrono>

#include <core/time.hpp>

namespace hyped::utils {

ManualTime::ManualTime() : current_time_{std::chrono::system_clock::from_time_t(0)}
{
}

core::TimePoint ManualTime::now() const
{
  return current_time_;
}

void ManualTime::set_time(const core::TimePoint time_point)
{
  current_time_ = time_point;
}

void ManualTime::set_seconds_since_epoch(const std::uint64_t seconds_since_epoch)
{
  current_time_ = std::chrono::system_clock::time_point(std::chrono::seconds(seconds_since_epoch));
}

}  // namespace hyped::utils
