#pragma once

#include "core/logger.hpp"
#include "core/types.hpp"
#include <navigation/control/consts.hpp>

namespace hyped::navigation {

class OpticalPreprocessor {
 public:
  OpticalPreprocessor(core::ILogger &log_);
  std::optional<std::array<core::Float, 2>> processData(const core::OpticalData raw_optical_data);

 private:
  core::ILogger &log_;
};

}  // namespace hyped::navigation