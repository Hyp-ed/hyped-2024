#pragma once

#include <cstdint>
#include <optional>

#include <core/logger.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

// TODOLater: Implement this properly
/**
 * @brief ADC (Texas Instruments) to be used for the hall-effect sensors (Farnell)
 * @details This class is used to convert the analog signal from the hall-effect sensors to a
 * digital signal over I2c
 */
class I2cConverter {
 public:
  static std::optional<I2cConverter> create(core::ILogger &logger);
  ~I2cConverter();

 private:
  I2cConverter();

 private:
  core::ILogger &logger_;
};

}  // namespace hyped::sensors