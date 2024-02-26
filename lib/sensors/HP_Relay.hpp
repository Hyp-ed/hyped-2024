#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {

class HPRelay {
 public:
    HPRelay(core::ILogger &logger);

 virtual std::optional<std::shared_ptr<HPRelayWriter>> getWriter(const std::uint8_t pin,
                                                                const Edge edge);

 private:
  /**
   * @brief Initialises the GPIO pin for reading or writing.
   * @param pin The pin to initialise.
   * @param edge The edge to trigger on. Defaults to "both".
   * @param direction The direction of the pin.
   */
  core::Result initialisePin(const std::uint8_t pin, const Edge edge, const Direction direction);
};

class HPRelayWriter : public IHPRelayWriter {
 public:
  HPRelayWriter(core::ILogger &logger, const int write_file_descriptor);
  ~HPRelayWriter();

  /**
   * @brief Writes a high or low to the GPIO pin.
   * @param state The digital signal to write to the pin.
   */
  virtual core::Result write(const core::DigitalSignal state);

 private:
  core::ILogger &logger_;
  const int write_file_descriptor_;
};

}  // namespace hyped::sensors