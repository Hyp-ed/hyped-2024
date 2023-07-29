#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/logger.hpp>
#include <io/can.hpp>
#include <io/hardware_adc.hpp>
#include <io/hardware_can.hpp>
#include <io/hardware_gpio.hpp>
#include <io/hardware_i2c.hpp>
#include <io/hardware_spi.hpp>
#include <io/hardware_uart.hpp>
#include <io/pwm.hpp>
#include <motors/constant_frequency_calculator.hpp>
#include <motors/controller.hpp>
#include <motors/frequency_calculator.hpp>
#include <motors/time_frequency_calculator.hpp>
#include <sensors/accelerometer.hpp>
#include <sensors/bms.hpp>
#include <sensors/temperature.hpp>

namespace hyped::debug {

struct Command {
  std::string name;
  std::string description;
  std::function<void(void)> handler;
};

class Repl {
 public:
  Repl(core::ILogger &logger);
  void run();
  std::optional<std::unique_ptr<Repl>> fromFile(const std::string &filename);

 private:
  void printCommands();
  void handleCommand();
  void addCommand(const Command &cmd);

  void addQuitCommand();
  void addHelpCommand();
  void addAdcCommands(const std::uint8_t pin);
  void addCanCommands(const std::string &bus);
  void addGpioReadCommands(const std::uint8_t pin);
  void addGpioWriteCommands(const std::uint8_t pin);
  void addI2cCommands(const std::uint8_t bus);
  void addPwmCommands(const std::uint8_t module, const std::uint32_t period);
  void addSpiCommands(const std::uint8_t bus);
  void addAccelerometerCommands(const std::uint8_t bus, const std::uint8_t device_address);
  void addTemperatureCommands(const std::uint8_t bus, const std::uint8_t device_address);
  void addUartCommands(const std::uint8_t bus);
  void addMotorControllerCommands(const std::string &bus);
  void addBmsCommands(const std::string &bus);

  /**
   * @brief Get the Adc object associated with the given pin or create a new one if it doesn't exist
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

  core::ILogger &logger_;
  std::map<std::string, Command> command_map_;
  std::unordered_map<std::uint8_t, std::shared_ptr<io::IAdc>> adc_;
  std::unordered_map<std::string, std::shared_ptr<io::ICan>> can_;
  io::HardwareGpio gpio_;
  std::unordered_map<std::uint8_t, std::shared_ptr<io::II2c>> i2c_;
  std::unordered_map<io::PwmModule, std::shared_ptr<io::Pwm>> pwm_;
  std::unordered_map<io::SpiBus, std::shared_ptr<io::ISpi>> spi_;
  std::unordered_map<io::UartBus, std::shared_ptr<io::IUart>> uart_;
};

}  // namespace hyped::debug