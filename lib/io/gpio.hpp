#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>

namespace hyped::io {

/**
 * An abstract interface to read from a GPIO pin. This is to be used whenever read access
 * to GPIO is required.
 */
class IGpioReader {
 public:
  virtual std::optional<core::DigitalSignal> read() = 0;
};

/**
 * An abstract interface to write to a GPIO pin. This is to be used whenever write access
 * to GPIO is required.
 */
class IGpioWriter {
 public:
  virtual core::Result write(const core::DigitalSignal state) = 0;
};

/**
 * An abstract GPIO interface. This is to be used in all places where it is necessary to
 * initiate GPIO access. It is the callees responsibility to provide a correct implementation
 * such as `Gpio` in `hardware_gpio.hpp`.
 */
class IGpio {
 public:
  virtual std::optional<std::shared_ptr<IGpioReader>> getReader(const std::uint8_t pin) = 0;
  virtual std::optional<std::shared_ptr<IGpioWriter>> getWriter(const std::uint8_t pin) = 0;
};

}  // namespace hyped::io
