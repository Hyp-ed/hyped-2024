#include "preprocess_optical.hpp"

#include <cstdint>

namespace hyped::navigation {

OpticalPreprocessor::OpticalPreprocessor(core::ILogger &logger) : log_(logger)
{
}

std::optional<std::array<core::Float, 2>> processData(const core::OpticalData raw_optical_data)
{
  // Take magnitude
  std::array<core::Float, 2> optical_data;
  for (std::size_t i = 0; i < core::kNumOptical; ++i) {
    core::Float magnitude = 0;
    for (std::size_t j = 0; j < 2; ++j) {
      magnitude += std::pow(raw_optical_data.at(i).at(j), 2);
    }
    optical_data.at(i) = std::sqrt(magnitude);
  }

  return optical_data;
}

}  // namespace hyped::navigation