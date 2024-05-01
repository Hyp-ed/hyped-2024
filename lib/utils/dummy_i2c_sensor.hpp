#pragma once

#include <sensors/i2c_sensors.hpp>

namespace hyped::utils {

class DummyI2cSensor : public sensors::II2cMuxSensor<std::uint8_t> {
 public:
  DummyI2cSensor() = default;
  virtual core::Result configure();
  std::optional<std::uint8_t> read() override;
  std::uint8_t getChannel() const override;
};

}  // namespace hyped::utils
