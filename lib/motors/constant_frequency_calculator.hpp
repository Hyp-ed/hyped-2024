#pragma once

#include "frequency_calculator.hpp"

#include <cstdint>

#include <core/logger.hpp>
#include <core/types.hpp>

namespace hyped::motors {

class ConstantFrequencyCalculator : public IFrequencyCalculator {
 public:
  ConstantFrequencyCalculator(core::ILogger &logger);
  /**
   * @brief Returns the passed in velocity as the frequency
   *
   * @param velocity not used, required by interface
   * @return frequency_
   */
  std::uint32_t calculateFrequency(core::Float velocity);

  /**
   * @brief Sets the frequency to be returned by calculateFrequency
   *
   * @param frequency the frequency to be returned by calculateFrequency
   */
  void setFrequency(std::uint16_t frequency);

 private:
  core::ILogger &logger_;
  std::uint32_t frequency_;
};

}  // namespace hyped::motors
