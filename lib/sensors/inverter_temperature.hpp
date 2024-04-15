#pragma once

#include "adc_mux.hpp"

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

class InverterTemperature {
 public:
  InverterTemperature(core::ILogger &logger,
                  std::shared_ptr<io::II2c> i2c,
                  const AdcMuxChannel adc_mux_channel);

  ~InverterTemperature();

  std::optional<core::Float> readTemperature();

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const AdcMuxChannel adc_mux_channel_;
};

}  // namespace hyped::sensors