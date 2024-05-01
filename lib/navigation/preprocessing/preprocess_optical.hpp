#pragma once

#include <core/logger.hpp>
#include <core/types.hpp>
#include <navigation/control/consts.hpp>

namespace hyped::navigation {

class OpticalPreprocessor {
 public:
  explicit OpticalPreprocessor(core::ILogger &logger_);
  std::optional<std::array<core::Float, 2>> processData(const core::OpticalData raw_optical_data);

 private:
  core::ILogger &logger_;
};

}  // namespace hyped::navigation
