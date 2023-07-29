#pragma once

#include "adc.hpp"

#include <cstdio>
#include <cstdlib>  // for atoi
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>

namespace hyped::io {

class HardwareAdc : public IAdc {
 public:
  /**
   * @brief Creates an Adc instance
   * @param pin is one of the 7 analogue input pins on the bbb
   */
  static std::optional<std::shared_ptr<HardwareAdc>> create(core::ILogger &logger,
                                                            const std::uint8_t pin);
  HardwareAdc(core::ILogger &logger, const int file_descriptor);
  ~HardwareAdc();

  virtual std::optional<core::Float> readValue();

 private:
  /**
   * @param    file_descriptor specifying the file voltage values are read from
   * @return   std::uint16_t returns two bytes of current voltage data
   */
  std::optional<core::Float> resetAndRead4(const int file_descriptor);

 private:
  core::ILogger &logger_;
  std::uint8_t pin_;
  const int file_descriptor_;

 private:
  static constexpr std::uint16_t kMaxAdcRawValue = 4096;
  static constexpr core::Float kMaxAdcVoltage    = 1.8;
};

}  // namespace hyped::io
