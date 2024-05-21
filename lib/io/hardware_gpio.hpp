#pragma once

#include "gpio.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <core/types.hpp>
#include <gpiod.hpp>

namespace hyped::io {

class HardwareGpioReader : public IGpioReader {
 public:
  HardwareGpioReader(core::ILogger &logger, gpiod::chip &chip, const std::uint8_t pin);

  /**
   * @brief Read a high or low from the GPIO pin.
   */
  std::optional<core::DigitalSignal> read() override;

 private:
  core::ILogger &logger_;
  gpiod::chip &chip_;
  const std::uint8_t pin_;
};

class HardwareGpioWriter : public IGpioWriter {
 public:
  HardwareGpioWriter(core::ILogger &logger, gpiod::chip &chip, const std::uint8_t pin);

  /**
   * @brief Writes a high or low to the GPIO pin.
   * @param state The digital signal to write to the pin.
   */
  core::Result write(const core::DigitalSignal state) override;

 private:
  core::ILogger &logger_;
  gpiod::chip &chip_;
  const std::uint8_t pin_;
};

/**
 * Hardware GPIO interface, requires physical GPIO pins to be present. This should only
 * be instantiated at the top level and then provided to users through the IGpio interface.
 * Ensure inputted pins are defined as pin = 32*X + Y (GPIOX_Y)
 */
class HardwareGpio {
 public:
  std::optional<std::shared_ptr<IGpio>> create(const std::string &chip_name);

  HardwareGpio(core::ILogger &log, const std::string &chip_name);

  core::DigitalSignal read(const std::uint8_t pin);

  void write(const std::uint8_t pin, const core::DigitalSignal state);

 private:
  core::ILogger &logger_;
  gpiod::chip chip_;
};

}  // namespace hyped::io
