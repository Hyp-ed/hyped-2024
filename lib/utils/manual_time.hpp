#pragma once

#include <core/time.hpp>

namespace hyped::utils {

class ManualTime : public core::ITimeSource {
 public:
  ManualTime();
  core::TimePoint now() const override;

  void set_time(const core::TimePoint time_point);
  void set_seconds_since_epoch(const std::uint64_t seconds_since_epoch);

 private:
  core::TimePoint current_time_;
};

}  // namespace hyped::utils
