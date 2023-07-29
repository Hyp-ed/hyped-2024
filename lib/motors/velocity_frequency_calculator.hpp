#pragma once

#include "frequency_calculator.hpp"

#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/timer.hpp>

namespace hyped::motors {

class VelocityFrequencyCalculator : public IFrequencyCalculator {
 public:
  static std::optional<std::shared_ptr<VelocityFrequencyCalculator>> create(
    core::ILogger &logger, const std::string &coefficient_file_path);
  VelocityFrequencyCalculator(core::ILogger &logger, std::array<core::Float, 5> &coefficients);
  /**
   * @brief Calculates the frequency to send to the motor controller based on the velocity and
   * provided polynomial expression
   *
   * @return core::Float equal to the passed in frequency
   */
  std::uint32_t calculateFrequency(core::Float velocity);

 private:
  core::ILogger &logger_;
  std::array<core::Float, 5> &coefficients_;
};

}  // namespace hyped::motors
