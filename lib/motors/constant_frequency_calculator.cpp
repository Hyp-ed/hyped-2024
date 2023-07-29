#include "constant_frequency_calculator.hpp"

namespace hyped::motors {

ConstantFrequencyCalculator::ConstantFrequencyCalculator(core::ILogger &logger) : logger_(logger)
{
}

std::uint32_t ConstantFrequencyCalculator::calculateFrequency(core::Float velocity)
{
  return frequency_;
}

void ConstantFrequencyCalculator::setFrequency(std::uint16_t frequency)
{
  frequency_ = frequency;
}

}  // namespace hyped::motors