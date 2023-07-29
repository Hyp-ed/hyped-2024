#pragma once

#include "frequency_calculator.hpp"

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <core/logger.hpp>
#include <core/timer.hpp>

namespace hyped::motors {

class TimeFrequencyCalculator : public IFrequencyCalculator {
 public:
  static std::optional<std::shared_ptr<TimeFrequencyCalculator>> create(core::ILogger &logger,
                                                                        const std::string &path);
  TimeFrequencyCalculator(core::ILogger &logger,
                          std::vector<std::pair<std::uint64_t, std::uint32_t>> &frequency_table);
  /**
   * @brief Returns the number of seconds elapsed as frequency mod 100
   *
   * @return core::Float equal to the passed in frequency
   */
  std::uint32_t calculateFrequency(core::Float velocity);

  /**
   * @brief Resets the start time to the current time
   */
  void reset();

 private:
  core::ILogger &logger_;
  std::chrono::time_point<std::chrono::system_clock> start_time_;
  std::vector<std::pair<std::uint64_t, std::uint32_t>> frequency_table_;
};

}  // namespace hyped::motors
