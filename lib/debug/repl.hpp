#pragma once

#include "terminal.hpp"

#include <ncurses.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "commands/Command.hpp"
#include <core/logger.hpp>
#include <io/adc.hpp>
#include <io/can.hpp>
#include <io/gpio.hpp>
#include <io/hardware_gpio.hpp>
#include <io/hardware_spi.hpp>
#include <io/hardware_uart.hpp>
#include <io/i2c.hpp>
#include <io/pwm.hpp>
#include <io/spi.hpp>
#include <io/uart.hpp>
#include <toml++/toml.hpp>

namespace hyped::debug {

class Repl {
 public:
  static std::optional<std::shared_ptr<Repl>> create(core::ILogger &logger,
                                                     Terminal &terminal,
                                                     const std::string &filename);
  Repl(core::ILogger &logger, Terminal &terminal);
  void run();
  std::vector<std::string> autoComplete(const std::string &partial);

  void addCommand(std::unique_ptr<Command> command);
  void printHelp();

  void addHelpCommand();
  void addQuitCommand();

  /**
   * @brief Get the Adc object associated with the given pin or create a new one if it
   * doesn't exist
   * @param pin target pin for the Adc object
   * @return std::optional<std::shared_ptr<io::IAdc>> containing the Adc object at pin or
   * std::nullopt if the Adc could not be created
   */
  std::optional<std::shared_ptr<io::IAdc>> getAdc(const std::uint8_t pin);
  /**
   * @brief Get the Can object associated with the given bus or create a new one if it doesn't exist
   * @param bus target bus for the Can object
   * @return std::optional<std::shared_ptr<io::ICan>> containing the Can object at bus or
   * std::nullopt if the Can could not be created
   */
  std::optional<std::shared_ptr<io::ICan>> getCan(const std::string &bus);
  /**
   * @brief Get the I2c object associated with the given bus or create a new one if it doesn't exist
   * @param bus target bus for the I2c object
   * @return std::optional<std::shared_ptr<io::II2c>> containing the I2c object at bus or
   * std::nullopt if the I2c could not be created
   */
  std::optional<std::shared_ptr<io::II2c>> getI2c(const std::uint8_t bus);
  /**
   * @brief Get the Pwm object associated with the given module or create a new one if it doesn't
   * exist
   * @param pwm_module target module for the Pwm object
   * @param period passed to Pwm::create
   * @param polarity passed to Pwm::create
   * @return std::optional<std::shared_ptr<io::Pwm>> containing the Pwm object at pwm_module or
   * std::nullopt if the Pwm could not be created WARNING: The returned Pwm object is not guaranteed
   * to have the same period or polarity as the parameters passed
   */
  std::optional<std::shared_ptr<io::Pwm>> getPwm(const io::PwmModule pwm_module,
                                                 const std::uint32_t period,
                                                 const io::Polarity polarity);
  /**
   * @brief Get the Spi object associated with the given bus or create a new one if it doesn't exist
   *
   * @param bus target bus for the Spi object
   * @param mode passed to Spi::create
   * @param word_size passed to Spi::create
   * @param bit_order passed to Spi::create
   * @param clock passed to Spi::create
   * @return std::optional<std::shared_ptr<io::ISpi>> containing the Spi object at bus or
   * std::nullopt if the Spi could not be created WARNING: The returned Spi object is not guaranteed
   * to have the same mode, word_size, bit_order or clock as the parameters passed
   */
  std::optional<std::shared_ptr<io::ISpi>> getSpi(const io::SpiBus bus,
                                                  const io::SpiMode mode,
                                                  const io::SpiWordSize word_size,
                                                  const io::SpiBitOrder bit_order,
                                                  const io::SpiClock clock);
  /**
   * @brief Get the Uart object associated with the given bus or create a new one if it doesn't
   * exist
   *
   * @param bus target bus for the Uart object
   * @param baud_rate passed to Uart::create
   * @param bits_per_byte passed to Uart::create
   * @return std::optional<std::shared_ptr<io::IUart>> containing the Uart object at bus or
   * std::nullopt if the Uart could not be created WARNING: The returned Uart object is not
   * guaranteed to have the same baud_rate or bits_per_byte as the parameters passed
   */
  std::optional<std::shared_ptr<io::IUart>> getUart(const io::UartBus bus,
                                                    const io::UartBaudRate baud_rate,
                                                    const io::UartBitsPerByte bits_per_byte);

 private:
  core::ILogger &logger_;
  Terminal terminal_;
  std::vector<std::string> history_;
  std::vector<std::unique_ptr<Command>> commands_;

  std::unordered_map<std::uint8_t, std::shared_ptr<io::IAdc>> adc_;
  std::unordered_map<std::string, std::shared_ptr<io::ICan>> can_;
  io::HardwareGpio gpio_;
  std::unordered_map<std::uint8_t, std::shared_ptr<io::II2c>> i2c_;
  std::unordered_map<io::PwmModule, std::shared_ptr<io::Pwm>> pwm_;
  std::unordered_map<io::SpiBus, std::shared_ptr<io::ISpi>> spi_;
  std::unordered_map<io::UartBus, std::shared_ptr<io::IUart>> uart_;
};
}  // namespace hyped::debug