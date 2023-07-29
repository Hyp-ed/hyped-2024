#pragma once

#include <optional>

#include <core/types.hpp>

namespace hyped::io {

class IAdc {
 public:
  /**
   * @brief reads AIN value
   * @return a voltage value from 0 to 1.8V
   */
  virtual std::optional<core::Float> readValue() = 0;
};

}  // namespace hyped::io
