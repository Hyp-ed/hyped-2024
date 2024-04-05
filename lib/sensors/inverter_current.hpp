#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {

class InverterCurrent {
 public:
  static std::optional<InverterCurrent> create(core::ILogger &logger,
                                               std::shared_ptr<io::II2c> i2c,
                                               const std::uint8_t adc_mux_channel);
  ~InverterCurrent();

  std::optional<core::Float> readCurrent();

 private:
  InverterCurrent(core::ILogger &logger,
                  std::shared_ptr<io::II2c> i2c,
                  const std::uint8_t adc_mux_channel);

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t adc_mux_channel_;
};

}  // namespace hyped::sensors