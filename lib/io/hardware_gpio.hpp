#pragma once

#include "gpio.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <core/types.hpp>

namespace hyped::io {

class HardwareGpioReader : public IGpioReader {
 public:
  HardwareGpioReader(core::ILogger &logger, const int read_file_descriptor);
  ~HardwareGpioReader();
  /**
   * @brief Read a high or low from the GPIO pin.
   */
  std::optional<core::DigitalSignal> read() override;

 private:
  core::ILogger &logger_;
  const int read_file_descriptor_;
};

class HardwareGpioWriter : public IGpioWriter {
 public:
  HardwareGpioWriter(core::ILogger &logger, const int write_file_descriptor);
  ~HardwareGpioWriter();

  /**
   * @brief Writes a high or low to the GPIO pin.
   * @param state The digital signal to write to the pin.
   */
  core::Result write(const core::DigitalSignal state) override;

 private:
  core::ILogger &logger_;
  const int write_file_descriptor_;
};

/**
 * Hardware GPIO interface, requires physical GPIO pins to be present. This should only
 * be instantiated at the top level and then provided to users through the IGpio interface.
 * Ensure inputted pins are defined as pin = 32*X + Y (GPIOX_Y)
 */
class HardwareGpio : public IGpio {
 public:
  explicit HardwareGpio(core::ILogger &logger);

  std::optional<std::shared_ptr<IGpioReader>> getReader(const std::uint8_t pin,
                                                        const Edge edge) override;
  std::optional<std::shared_ptr<IGpioWriter>> getWriter(const std::uint8_t pin,
                                                        const Edge edge) override;

 private:
  /**
   * @brief Initialises the GPIO pin for reading or writing.
   * @param pin The pin to initialise.
   * @param edge The edge to trigger on. Defaults to "both".
   * @param direction The direction of the pin.
   */
  core::Result initialisePin(const std::uint8_t pin, const Edge edge, const Direction direction);

  /**
   * @brief Exports the GPIO pin to the filesystem.
   * @details This is required to be able to access the pin. Normally hidden from userspace.
   * @param pin The pin to export.
   */
  core::Result exportPin(const std::uint8_t pin);

  /**
   * @brief Get the file descriptor for the pin depending on if we are reading or writing.
   */
  int getFileDescriptor(const std::uint8_t pin, const Direction direction);

  // Helper functions to get the string representation of the edge and direction.
  static std::string getEdgeString(const Edge edge);
  static std::string getDirectionString(const Direction direction);

  core::ILogger &logger_;
};

}  // namespace hyped::io
