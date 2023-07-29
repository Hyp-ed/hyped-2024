#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/adc.hpp>

namespace hyped::sensors {

// Skeleton code for the thermistor class
// TODOLater: Implement this properly
class Thermistor {
 public:
  static std::optional<Thermistor> create(core::ILogger &logger, std::shared_ptr<io::IAdc> adc);
  ~Thermistor();

 private:
  Thermistor(core::ILogger &logger, std::shared_ptr<io::IAdc> adc);

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::IAdc> adc_;
};

}  // namespace hyped::sensors