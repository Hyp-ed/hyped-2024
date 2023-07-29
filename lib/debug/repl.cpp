#include "repl.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>

namespace hyped::debug {

Repl::Repl(core::ILogger &logger)
    : logger_(logger),
      i2c_(),
      spi_(),
      pwm_(),
      adc_(),
      uart_(),
      gpio_(io::HardwareGpio(logger))
{
}

void Repl::run()
{
  while (true) {
    handleCommand();
  }
}

std::optional<std::unique_ptr<Repl>> Repl::fromFile(const std::string &path)
{
  std::ifstream input_stream(path);
  if (!input_stream.is_open()) {
    logger_.log(core::LogLevel::kFatal, "Failed to open file %s", path.c_str());
    return std::nullopt;
  }

  rapidjson::IStreamWrapper input_stream_wrapper(input_stream);
  rapidjson::Document document;
  rapidjson::ParseResult result = document.ParseStream(input_stream_wrapper);
  if (!result) {
    logger_.log(core::LogLevel::kFatal,
                "Error parsing JSON: %s",
                rapidjson::GetParseError_En(document.GetParseError()));
    return std::nullopt;
  }

  if (!document.HasMember("debugger")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'debugger' in configuration file at %s",
                path.c_str());
    return std::nullopt;
  }
  const auto debugger = document["debugger"].GetObject();
  auto repl           = std::make_unique<Repl>(logger_);
  repl->addHelpCommand();
  repl->addQuitCommand();

  if (!debugger.HasMember("io")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'debugger.io' in configuration file at %s",
                path.c_str());
    return std::nullopt;
  }
  const auto io = debugger["io"].GetObject();
  if (!io.HasMember("adc")) {
    logger_.log(core::LogLevel::kFatal, "Missing required field 'io.adc' in configuration file");
    return std::nullopt;
  }
  const auto adc = io["adc"].GetObject();
  if (!adc.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'io.adc.enabled' in configuration file");
    return std::nullopt;
  }
  if (adc["enabled"].GetBool()) {
    if (!adc.HasMember("pins")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.adc.pins' in configuration file");
      return std::nullopt;
    }
    const auto pins = adc["pins"].GetArray();
    for (auto &pin : pins) {
      repl->addAdcCommands(pin.GetUint());
    }
  }
  if (!io.HasMember("can")) {
    logger_.log(core::LogLevel::kFatal, "Missing required field 'io.can' in configuration file");
    return std::nullopt;
  }
  const auto can = io["can"].GetObject();
  if (!can.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'io.can.enabled' in configuration file");
    return std::nullopt;
  }
  if (can["enabled"].GetBool()) {
    if (!can.HasMember("buses")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.can.buses' in configuration file");
      return std::nullopt;
    }
    const auto buses = can["buses"].GetArray();
    for (auto &bus : buses) {
      repl->addCanCommands(bus.GetString());
    }
  }
  if (!io.HasMember("i2c")) {
    logger_.log(core::LogLevel::kFatal, "Missing required field 'io.i2c' in configuration file");
    return std::nullopt;
  }
  const auto i2c = io["i2c"].GetObject();
  if (!i2c.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'io.i2c.enabled' in configuration file");
    return std::nullopt;
  }
  if (i2c["enabled"].GetBool()) {
    if (!i2c.HasMember("buses")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.i2c.buses' in configuration file");
      return std::nullopt;
    }
  }
  if (!io.HasMember("gpio")) {
    logger_.log(core::LogLevel::kFatal, "Missing required field 'io.gpio' in configuration file");
    return std::nullopt;
  }
  const auto gpio = io["gpio"].GetObject();
  if (!gpio.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'io.gpio.enabled' in configuration file");
    return std::nullopt;
  }
  if (gpio["enabled"].GetBool()) {
    if (!gpio.HasMember("read_pins")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.gpio.read_pins' in configuration file");
      return std::nullopt;
    }
    const auto read_pins = gpio["read_pins"].GetArray();
    for (auto &pin : read_pins) {
      repl->addGpioReadCommands(pin.GetUint());
    }
    if (!gpio.HasMember("write_pins")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.gpio.write_pins' in configuration file");
      return std::nullopt;
    }
    const auto write_pins = gpio["write_pins"].GetArray();
    for (auto &pin : write_pins) {
      repl->addGpioWriteCommands(pin.GetUint());
    }
  }
  const auto buses = i2c["buses"].GetArray();
  for (auto &bus : buses) {
    repl->addI2cCommands(bus.GetUint());
  }
  if (!io.HasMember("pwm")) {
    logger_.log(core::LogLevel::kFatal, "Missing required field 'io.pwm' in configuration file");
    return std::nullopt;
  }
  const auto pwm = io["pwm"].GetObject();
  if (!pwm.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'io.pwm.enabled' in configuration file");
    return std::nullopt;
  }
  if (pwm["enabled"].GetBool()) {
    if (!pwm.HasMember("modules")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.pwm.modules' in configuration file");
      return std::nullopt;
    }
    const auto modules = pwm["modules"].GetArray();
    if (!pwm.HasMember("period")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.pwm.period' in configuration file");
      return std::nullopt;
    }
    const std::uint32_t period = pwm["period"].GetUint();
    for (auto &module : modules) {
      const std::uint8_t module_id = module.GetUint();
      if (module_id > 7 || module_id < 0) {
        logger_.log(
          core::LogLevel::kFatal, "Invalid module id %d in configuration file", module_id);
        return std::nullopt;
      }
      repl->addPwmCommands(module_id, period);
    }
  }
  if (!io.HasMember("spi")) {
    logger_.log(core::LogLevel::kFatal, "Missing required field 'io.spi' in configuration file");
    return std::nullopt;
  }
  const auto spi = io["spi"].GetObject();
  if (!spi.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'io.spi.enabled' in configuration file");
    return std::nullopt;
  }
  if (spi["enabled"].GetBool()) {
    if (!spi.HasMember("buses")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.spi.buses' in configuration file");
      return std::nullopt;
    }
    const auto buses = spi["buses"].GetArray();
    for (auto &bus : buses) {
      repl->addSpiCommands(bus.GetUint());
    }
  }
  if (!io.HasMember("uart")) {
    logger_.log(core::LogLevel::kFatal, "Missing required field 'io.uart' in configuration file");
    return std::nullopt;
  }
  const auto uart = io["uart"].GetObject();
  if (!uart.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'io.uart.enabled' in configuration file");
    return std::nullopt;
  }
  if (uart["enabled"].GetBool()) {
    if (!uart.HasMember("buses")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'io.uart.buses' in configuration file");
      return std::nullopt;
    }
    const auto buses = uart["buses"].GetArray();
    for (auto &bus : buses) {
      repl->addUartCommands(bus.GetUint());
    }
  }
  if (!debugger.HasMember("sensors")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'debugger.sensors' in configuration file at %s",
                path.c_str());
    return std::nullopt;
  }
  const auto sensors = debugger["sensors"].GetObject();
  if (!sensors.HasMember("accelerometer")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'sensors.accelerometer' in configuration file");
    return std::nullopt;
  }
  const auto accelerometer = sensors["accelerometer"].GetObject();
  if (!accelerometer.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'sensors.accelerometer.enabled' in configuration file");
    return std::nullopt;
  }
  if (accelerometer["enabled"].GetBool()) {
    if (!accelerometer.HasMember("bus")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'sensors.accelerometer.bus' in configuration file");
      return std::nullopt;
    }
    if (!accelerometer.HasMember("device_address")) {
      logger_.log(
        core::LogLevel::kFatal,
        "Missing required field 'sensors.accelerometer.device_address' in configuration file");
      return std::nullopt;
    }
    const auto device_address = accelerometer["device_address"].GetUint();
    const auto bus            = accelerometer["bus"].GetUint();
    repl->addAccelerometerCommands(bus, device_address);
  }
  if (!sensors.HasMember("temperature")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'sensors.temperature' in configuration file");
    return std::nullopt;
  }
  const auto temperature = sensors["temperature"].GetObject();
  if (!temperature.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'sensors.temperature.enabled' in configuration file");
    return std::nullopt;
  }
  if (temperature["enabled"].GetBool()) {
    if (!temperature.HasMember("bus")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'sensors.temperature.bus' in configuration file");
      return std::nullopt;
    }
    if (!temperature.HasMember("device_address")) {
      logger_.log(
        core::LogLevel::kFatal,
        "Missing required field 'sensors.temperature.device_address' in configuration file");
      return std::nullopt;
    }
    const auto device_address = temperature["device_address"].GetUint();
    const auto bus            = temperature["bus"].GetUint();
    repl->addTemperatureCommands(bus, device_address);
  }
  if (!sensors.HasMember("bms")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'sensors.bms' in configuration file");
    return std::nullopt;
  }
  const auto bms = sensors["bms"].GetObject();
  if (!bms.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'sensors.bms.enabled' in configuration file");
    return std::nullopt;
  }
  if (bms["enabled"].GetBool()) {
    if (!bms.HasMember("bus")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'sensors.bms.bus' in configuration file");
      return std::nullopt;
    }
    const auto bus = bms["bus"].GetString();
    repl->addBmsCommands(bus);
  }
  if (!debugger.HasMember("motors")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'debugger.motors' in configuration file");
    return std::nullopt;
  }
  const auto motors = debugger["motors"].GetObject();
  if (!motors.HasMember("motor_controller")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'debugger.motor_controller' in configuration file");
    return std::nullopt;
  }
  const auto motor_controller = motors["motor_controller"].GetObject();
  if (!motor_controller.HasMember("enabled")) {
    logger_.log(core::LogLevel::kFatal,
                "Missing required field 'motor_controller.enabled' in configuration file");
    return std::nullopt;
  }
  if (motor_controller["enabled"].GetBool()) {
    if (!motor_controller.HasMember("bus")) {
      logger_.log(core::LogLevel::kFatal,
                  "Missing required field 'motor_controller.bus' in configuration file");
      return std::nullopt;
    }
    const auto bus = motor_controller["bus"].GetString();
    repl->addMotorControllerCommands(bus);
  }
  return repl;
}

void Repl::printCommands()
{
  logger_.log(core::LogLevel::kInfo, "Available commands:");
  for (const auto &[name, command] : command_map_) {
    logger_.log(core::LogLevel::kInfo, "  %s: %s", name.c_str(), command.description.c_str());
  }
}

void Repl::handleCommand()
{
  std::cout << "> ";
  std::string command;
  std::getline(std::cin, command);
  auto nameAndCommand = command_map_.find(command);
  if (nameAndCommand == command_map_.end()) {
    logger_.log(core::LogLevel::kFatal, "Unknown command: %s", command.c_str());
    return;
  }
  nameAndCommand->second.handler();
}

void Repl::addCommand(const Command &cmd)
{
  command_map_.emplace(cmd.name, cmd);
  logger_.log(core::LogLevel::kDebug, "Added command: %s", cmd.name.c_str());
}

void Repl::addQuitCommand()
{
  addCommand(Command{"quit", "Quit the REPL", [this]() { exit(0); }});
}

void Repl::addHelpCommand()
{
  addCommand(Command{"help", "Print this help message", [this]() { printCommands(); }});
}

void Repl::addAdcCommands(const std::uint8_t pin)
{
  const auto optional_adc = getAdc(pin);
  if (!optional_adc) {
    logger_.log(core::LogLevel::kFatal, "Failed to create ADC instance on pin %d", pin);
    return;
  }
  const auto adc = std::move(*optional_adc);
  Command adc_read_command;
  std::stringstream identifier;
  identifier << "adc " << static_cast<int>(pin) << " read";
  adc_read_command.name = identifier.str();
  std::stringstream description;
  description << "Read from ADC pin " << static_cast<int>(pin);
  adc_read_command.description = description.str();
  adc_read_command.handler     = [this, adc, pin]() {
    const auto value = adc->readValue();
    if (!value) {
      logger_.log(core::LogLevel::kFatal, "Failed to read from ADC pin %d", pin);
      return;
    }
    logger_.log(core::LogLevel::kDebug, "ADC value from pin %d: %f", pin, *value);
  };
  addCommand(adc_read_command);
}

void Repl::addCanCommands(const std::string &bus)
{
  const auto optional_can = io::HardwareCan::create(logger_, bus);
  if (!optional_can) {
    logger_.log(core::LogLevel::kFatal, "Failed to create CAN instance on bus %s", bus.c_str());
    return;
  }
  const auto can = std::move(*optional_can);
  Command can_write_command;
  std::stringstream identifier;
  identifier << bus << " write";
  can_write_command.name = identifier.str();
  std::stringstream description;
  description << "Write to CAN bus " << bus;
  can_write_command.description = description.str();
  can_write_command.handler     = [this, can, bus]() {
    std::cout << "Enter CAN ID: ";
    std::uint32_t id;
    std::cin >> std::hex >> id;
    std::cout << "Enter CAN data: ";
    std::string data;
    std::cin >> std::hex >> data;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (data.length() > 16) {
      logger_.log(core::LogLevel::kFatal, "Cannot send can data longer than 8 bytes");
      return;
    }
    const auto can_data = std::vector<std::uint8_t>(data.begin(), data.end());
    io::CanFrame can_frame;
    can_frame.can_id  = id;
    can_frame.can_dlc = can_data.size();
    for (int i = 0; i < can_data.size(); i++) {
      can_frame.data[i] = can_data[i];
    }
    core::Result result = can->send(can_frame);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to write to CAN bus %s", bus.c_str());
      return;
    }
    logger_.log(core::LogLevel::kDebug, "Wrote to CAN bus %s", bus.c_str());
  };
  addCommand(can_write_command);
}

void Repl::addGpioReadCommands(const std::uint8_t pin)
{
  const auto optional_gpio_reader = gpio_.getReader(pin, io::Edge::kNone);
  if (!optional_gpio_reader) {
    logger_.log(core::LogLevel::kFatal, "Failed to create GPIO reader on pin %d", pin);
    return;
  }
  const auto gpio_reader = std::move(*optional_gpio_reader);
  Command gpio_read_command;
  std::stringstream identifier;
  identifier << "gpio " << static_cast<int>(pin) << " read";
  gpio_read_command.name = identifier.str();
  std::stringstream description;
  description << "Read from GPIO pin " << static_cast<int>(pin);
  gpio_read_command.description = description.str();
  gpio_read_command.handler     = [this, gpio_reader, pin]() {
    const auto value = gpio_reader->read();
    if (!value) {
      logger_.log(core::LogLevel::kFatal, "Failed to read from GPIO pin %d", pin);
      return;
    }
    logger_.log(
      core::LogLevel::kDebug, "GPIO value from pin %d: %d", pin, static_cast<std::uint8_t>(*value));
  };
}

void Repl::addGpioWriteCommands(const std::uint8_t pin)
{
  const auto optional_gpio_writer = gpio_.getWriter(pin, io::Edge::kNone);
  if (!optional_gpio_writer) {
    logger_.log(core::LogLevel::kFatal, "Failed to create GPIO writer on pin %d", pin);
    return;
  }
  const auto gpio_writer = std::move(*optional_gpio_writer);
  Command gpio_write_command;
  std::stringstream identifier;
  identifier << "gpio " << static_cast<int>(pin) << " write";
  gpio_write_command.name = identifier.str();
  std::stringstream description;
  description << "Write to GPIO pin " << static_cast<int>(pin);
  gpio_write_command.description = description.str();
  gpio_write_command.handler     = [this, gpio_writer, pin]() {
    std::cout << "Enter GPIO value: ";
    std::uint16_t value;
    std::cin >> std::hex >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    core::DigitalSignal signal;
    switch (value) {
      case 0:
        signal = core::DigitalSignal::kLow;
        break;
      case 1:
        signal = core::DigitalSignal::kHigh;
        break;
      default:
        logger_.log(core::LogLevel::kFatal, "Invalid GPIO value: %d, must be 0 or 1", value);
        break;
    }
    const auto result = gpio_writer->write(signal);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to write to GPIO pin %d", pin);
      return;
    }
    logger_.log(core::LogLevel::kDebug, "Wrote %d to GPIO pin %d", value, pin);
  };
  addCommand(gpio_write_command);
}

void Repl::addI2cCommands(const std::uint8_t bus)
{
  const auto optional_i2c = getI2c(bus);
  if (!optional_i2c) {
    logger_.log(core::LogLevel::kFatal, "Failed to create I2C instance on bus %d", bus);
    return;
  }
  const auto i2c = std::move(*optional_i2c);
  {
    Command i2c_read_command;
    std::stringstream identifier;
    identifier << "i2c " << static_cast<int>(bus) << " read";
    i2c_read_command.name = identifier.str();
    std::stringstream description;
    description << "Read from I2C bus " << static_cast<int>(bus);
    i2c_read_command.description = description.str();
    i2c_read_command.handler     = [this, i2c, bus]() {
      std::uint16_t device_address, register_address;
      std::cout << "Device address: ";
      std::cin >> device_address;
      std::cout << "Register address: ";
      std::cin >> register_address;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      const auto value = i2c->readByte(device_address, register_address);
      if (value) {
        logger_.log(core::LogLevel::kDebug, "I2C value from bus %d: %d", bus, *value);
      } else {
        logger_.log(core::LogLevel::kFatal, "Failed to read from I2C bus %d", bus);
      }
    };
    addCommand(i2c_read_command);
  }
  {
    Command i2c_write_command;
    std::stringstream identifier;
    identifier << "i2c " << static_cast<int>(bus) << " write";
    i2c_write_command.name = identifier.str();
    std::stringstream description;
    description << "Write to I2C bus " << static_cast<int>(bus);
    i2c_write_command.description = description.str();
    i2c_write_command.handler     = [this, i2c, bus]() {
      std::uint32_t device_address, register_address, data;
      std::cout << "Device address: ";
      std::cin >> std::hex >> device_address;
      std::cout << "Register address: ";
      std::cin >> std::hex >> register_address;
      std::cout << "Data: ";
      std::cin >> std::hex >> data;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      const core::Result result = i2c->writeByteToRegister(device_address, register_address, data);
      if (result == core::Result::kSuccess) {
        logger_.log(
          core::LogLevel::kDebug, "I2C write successful to device %d on %d", device_address, bus);
      } else {
        logger_.log(core::LogLevel::kFatal, "Failed to write to I2C bus: %d", bus);
      }
    };
    addCommand(i2c_write_command);
  }
}

void Repl::addPwmCommands(const std::uint8_t module, const std::uint32_t period)
{
  const io::PwmModule pwm_module = static_cast<io::PwmModule>(module);
  const auto optional_pwm        = getPwm(pwm_module, period, io::Polarity::kActiveHigh);
  if (!optional_pwm) {
    logger_.log(core::LogLevel::kFatal, "Failed to create PWM module");
    return;
  }
  const auto pwm = std::move(*optional_pwm);
  {
    Command pwm_run_command;
    std::stringstream identifier;
    identifier << "pwm " << static_cast<int>(pwm_module) << " run";
    pwm_run_command.name = identifier.str();
    std::stringstream description;
    description << "Run PWM module: " << static_cast<int>(pwm_module);
    pwm_run_command.description = description.str();
    pwm_run_command.handler     = [this, pwm, pwm_module]() {
      core::Float duty_cycle;
      std::cout << "Duty cycle: ";
      std::cin >> duty_cycle;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      const core::Result duty_cycle_set_result = pwm->setDutyCycleByPercentage(duty_cycle);
      if (duty_cycle_set_result == core::Result::kFailure) {
        logger_.log(core::LogLevel::kFatal, "Failed to set PWM duty cycle");
        return;
      }
      const core::Result enable_result = pwm->setMode(io::Mode::kRun);
      if (enable_result == core::Result::kFailure) {
        logger_.log(core::LogLevel::kFatal, "Failed to enable PWM module");
        return;
      }
    };
    addCommand(pwm_run_command);
  }
  {
    Command pwm_stop_command;
    std::stringstream identifier;
    identifier << "pwm " << static_cast<int>(pwm_module) << " stop";
    pwm_stop_command.name = identifier.str();
    std::stringstream description;
    description << "Stop PWM module: " << static_cast<int>(pwm_module);
    pwm_stop_command.description = description.str();
    pwm_stop_command.handler     = [this, pwm, pwm_module]() {
      const core::Result disable_result = pwm->setMode(io::Mode::kStop);
      if (disable_result == core::Result::kFailure) {
        logger_.log(core::LogLevel::kFatal, "Failed to stop PWM module");
        return;
      }
    };
    addCommand(pwm_stop_command);
  }
}

void Repl::addSpiCommands(const std::uint8_t bus)
{
  const auto optional_spi = io::HardwareSpi::create(logger_,
                                                    io::SpiBus::kSpi1,
                                                    io::SpiMode::kMode3,
                                                    io::SpiWordSize::kWordSize8,
                                                    io::SpiBitOrder::kMsbFirst,
                                                    io::SpiClock::k500KHz);
  if (!optional_spi) {
    logger_.log(core::LogLevel::kFatal, "Failed to create I2C instance on bus %d", bus);
    return;
  }
  const auto spi = std::move(*optional_spi);
  {
    Command spi_read_byte_command;
    std::stringstream identifier;
    identifier << "spi " << static_cast<int>(bus) << " read byte";
    spi_read_byte_command.name = identifier.str();
    std::stringstream description;
    description << "Read from SPI bus " << static_cast<int>(bus);
    spi_read_byte_command.description = description.str();
    spi_read_byte_command.handler     = [this, spi, bus]() {
      std::uint16_t register_address;
      std::cout << "Register address: ";
      std::cin >> register_address;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::uint8_t read_buffer;
      const core::Result result = spi->read(register_address, &read_buffer, 1);
      if (result == core::Result::kSuccess) {
        logger_.log(core::LogLevel::kDebug, "SPI value from bus %d: %d", bus, read_buffer);
      } else {
        logger_.log(core::LogLevel::kFatal, "Failed to read from SPI bus %d", bus);
      }
    };
    addCommand(spi_read_byte_command);
  }
  {
    Command spi_write_byte_command;
    std::stringstream identifier;
    identifier << "spi " << static_cast<int>(bus) << " write byte";
    spi_write_byte_command.name = identifier.str();
    std::stringstream description;
    description << "Write to SPI bus " << static_cast<int>(bus);
    spi_write_byte_command.description = description.str();
    spi_write_byte_command.handler     = [this, spi, bus]() {
      std::uint32_t register_address, data;
      std::cout << "Register address: ";
      std::cin >> std::hex >> register_address;
      std::cout << "Data: ";
      std::cin >> std::hex >> data;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      const std::uint8_t *data_ptr = reinterpret_cast<const std::uint8_t *>(&data);
      const core::Result result    = spi->write(register_address, data_ptr, 1);
      if (result == core::Result::kSuccess) {
        logger_.log(
          core::LogLevel::kDebug, "Successful SPI write to device %d on %d", register_address, bus);
      } else {
        logger_.log(core::LogLevel::kFatal, "Failed to write to SPI bus: %d", bus);
      }
    };
    addCommand(spi_write_byte_command);
  }
}

void Repl::addUartCommands(const std::uint8_t bus)
{
  const io::UartBus uart_bus = static_cast<io::UartBus>(bus);
  const auto optional_uart = getUart(uart_bus, io::UartBaudRate::kB38400, io::UartBitsPerByte::k8);
  if (!optional_uart) {
    logger_.log(core::LogLevel::kFatal, "Failed to create UART instance on bus %d", bus);
    return;
  }
  const auto uart = std::move(*optional_uart);
  {
    Command uart_read_command;
    std::stringstream identifier;
    identifier << "uart " << static_cast<int>(bus) << " read";
    uart_read_command.name = identifier.str();
    std::stringstream description;
    description << "Read from UART bus " << static_cast<int>(bus);
    uart_read_command.description = description.str();
    uart_read_command.handler     = [this, uart, bus]() {
      std::uint8_t read_buffer[255];
      const core::Result result = uart->readBytes(read_buffer, 255);
      if (result == core::Result::kSuccess) {
        logger_.log(core::LogLevel::kDebug, "UART value from bus %d: %s", bus, read_buffer);
      } else {
        logger_.log(core::LogLevel::kFatal, "Failed to read from UART bus %d", bus);
      }
    };
    addCommand(uart_read_command);
  }
  {
    Command uart_write_command;
    std::stringstream identifier;
    identifier << "uart " << static_cast<int>(bus) << " write";
    uart_write_command.name = identifier.str();
    std::stringstream description;
    description << "Write to UART bus " << static_cast<int>(bus);
    uart_write_command.description = description.str();
    uart_write_command.handler     = [this, uart, bus]() {
      std::string data;
      std::cout << "Data: ";
      std::getline(std::cin, data);
      if (data.length() > 255) {
        logger_.log(core::LogLevel::kFatal, "Data too long for UART bus: %d", bus);
        return;
      }
      const core::Result result = uart->sendBytes(data.c_str(), data.length());
      if (result == core::Result::kSuccess) {
        logger_.log(core::LogLevel::kDebug, "Successfully wrote to UART bus %d", bus);
      } else {
        logger_.log(core::LogLevel::kFatal, "Failed to write to UART bus: %d", bus);
      }
    };
    addCommand(uart_write_command);
  }
}

void Repl::addAccelerometerCommands(const std::uint8_t bus, const std::uint8_t device_address)
{
  const auto optional_i2c = getI2c(bus);
  if (!optional_i2c) {
    logger_.log(core::LogLevel::kFatal, "Failed to create I2C instance on bus %d", bus);
    return;
  }
  const auto i2c = std::move(*optional_i2c);
  const auto optional_accelerometer
    = sensors::Accelerometer::create(logger_, i2c, bus, device_address);
  const auto accelerometer = std::make_shared<sensors::Accelerometer>(*optional_accelerometer);
  Command accelerometer_read_command;
  std::stringstream identifier;
  identifier << "accelerometer 0x" << std::hex << static_cast<int>(device_address) << " read";
  accelerometer_read_command.name = identifier.str();
  std::stringstream description;
  description << "Read accelerometer sensor 0x" << std::hex << static_cast<int>(device_address)
              << " on "
              << "I2C bus " << static_cast<int>(bus);
  accelerometer_read_command.description = description.str();
  accelerometer_read_command.handler     = [this, accelerometer, bus]() {
    const auto value = accelerometer->read();
    if (!value) {
      logger_.log(core::LogLevel::kFatal, "Failed to read the accelerometer from bus %d", bus);
    } else {
      const core::RawAccelerationData accelerometer_result = *value;
      logger_.log(core::LogLevel::kInfo,
                  "Acceleration in mg: \n x %d \n y %d \n z %d",
                  accelerometer_result.x,
                  accelerometer_result.y,
                  accelerometer_result.z);
    }
  };
  addCommand(accelerometer_read_command);
}

void Repl::addTemperatureCommands(const std::uint8_t bus, const std::uint8_t device_address)
{
  const auto optional_i2c = getI2c(bus);
  if (!optional_i2c) {
    logger_.log(core::LogLevel::kFatal, "Failed to create I2C instance on bus %d", bus);
    return;
  }
  const auto i2c                  = std::move(*optional_i2c);
  const auto optional_temperature = sensors::Temperature::create(logger_, i2c, bus, device_address);
  const auto temperature          = std::make_shared<sensors::Temperature>(*optional_temperature);
  Command temperature_read_command;
  std::stringstream identifier;
  identifier << "temperature 0x" << std::hex << static_cast<int>(device_address) << " read";
  temperature_read_command.name = identifier.str();
  std::stringstream description;
  description << "Read temperature sensor 0x" << std::hex << static_cast<int>(device_address)
              << " on "
              << "I2C bus " << static_cast<int>(bus);
  temperature_read_command.description = description.str();
  temperature_read_command.handler     = [this, temperature, bus]() {
    const auto value = temperature->read();
    if (!value) {
      logger_.log(core::LogLevel::kFatal, "Failed to read the temperature sensor from bus %d", bus);
    } else {
      const std::int16_t temperature_result = *value;
      logger_.log(core::LogLevel::kInfo, "Temperature in degrees Celsius: %d", temperature_result);
    }
  };
  addCommand(temperature_read_command);
}

void Repl::addMotorControllerCommands(const std::string &bus)
{
  const auto optional_can = getCan(bus);
  if (!optional_can) {
    logger_.log(core::LogLevel::kFatal, "Failed to create CAN instance on bus %s", bus.c_str());
    return;
  }
  const auto can                  = std::move(*optional_can);
  const auto frequency_calculator = std::make_shared<motors::ConstantFrequencyCalculator>(logger_);
  const auto optional_controller  = motors::Controller::create(
    logger_, "motor_controller_messages.json", can, frequency_calculator);
  if (!optional_controller) {
    logger_.log(core::LogLevel::kFatal, "Failed to create motor controller instance");
    return;
  }
  auto controller = std::move(*optional_controller);
  Command controller_read_register_command;
  controller_read_register_command.name = "controller read index";
  controller_read_register_command.description
    = "Read the value of the SDO object at the location specified by the index";
  controller_read_register_command.handler = [this, can]() {
    std::uint16_t index;
    std::cout << "Index: ";
    std::cin >> std::hex >> index;
    std::uint16_t sub_index;
    std::cout << "Sub-index: ";
    std::cin >> std::hex >> sub_index;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    io::CanFrame frame;
    frame.can_id  = motors::kControllerSdoSend;
    frame.can_dlc = 8;
    frame.data[0] = motors::kControllerSdoReadCommand;
    // set index
    frame.data[1] = index & 0x00FF;
    frame.data[2] = (index & 0xFF00) >> 8;
    // set sub-index
    frame.data[3] = static_cast<std::uint8_t>(sub_index);
    // set other values to 0
    frame.data[4]       = 0;
    frame.data[5]       = 0;
    frame.data[6]       = 0;
    frame.data[7]       = 0;
    core::Result result = can->send(frame);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to send SDO read request");
      return;
    }
  };
  addCommand(controller_read_register_command);
  Command controller_write_register_command;
  controller_write_register_command.name = "controller write index";
  controller_write_register_command.description
    = "Set the value of the SDO object at the location specified by the index";
  controller_write_register_command.handler = [this, can]() {
    std::uint16_t index;
    std::cout << "Index: ";
    std::cin >> std::hex >> index;
    std::uint16_t sub_index;
    std::cout << "Sub-index: ";
    std::cin >> std::hex >> sub_index;
    std::uint32_t value;
    std::cout << "Value: ";
    std::cin >> std::hex >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    io::CanFrame frame;
    frame.can_id  = motors::kControllerSdoSend;
    frame.can_dlc = 8;
    frame.data[0] = motors::kControllerSdoWriteCommand;
    // set index
    frame.data[1] = index & 0x00FF;
    frame.data[2] = (index & 0xFF00) >> 8;
    // set sub-index
    frame.data[3] = static_cast<std::uint8_t>(sub_index);
    // set value
    frame.data[4]       = value & 0x000000FF;
    frame.data[6]       = (value & 0x0000FF00) >> 8;
    frame.data[5]       = (value & 0x00FF0000) >> 16;
    frame.data[7]       = (value & 0xFF000000) >> 24;
    core::Result result = can->send(frame);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to send SDO write request");
      return;
    }
  };
  addCommand(controller_write_register_command);
  Command controller_configure_command;
  controller_configure_command.name = "controller configure";
  controller_configure_command.description
    = "Configure the motor controller with the configuration in the motor_controller_messages.json";
  controller_configure_command.handler = [this, controller]() {
    core::Result result = controller->run(motors::FauxState::kConfigure);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to configure the motor controller");
      return;
    }
  };
  addCommand(controller_configure_command);
  Command controller_set_frequency_command;
  controller_set_frequency_command.name        = "controller set frequency";
  controller_set_frequency_command.description = "Set the frequency of the motor controller in Hz";
  controller_set_frequency_command.handler     = [this, frequency_calculator]() {
    core::Float frequency;
    std::cout << "Frequency: ";
    std::cin >> frequency;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    frequency_calculator->setFrequency(frequency);
  };
  addCommand(controller_set_frequency_command);
  Command controller_run_command;
  controller_run_command.name        = "controller run";
  controller_run_command.description = "Enter running state and begin pwm";
  controller_run_command.handler     = [this, controller]() {
    core::Result result = controller->run(motors::FauxState::kAccelerate);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to run the motor controller");
      return;
    }
  };
  addCommand(controller_run_command);
  Command controller_stop_command;
  controller_stop_command.name        = "controller stop";
  controller_stop_command.description = "Enter stopped state and stop pwm";
  controller_stop_command.handler     = [this, controller]() {
    core::Result result = controller->run(motors::FauxState::kStop);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to stop the motor controller");
      return;
    }
  };
  addCommand(controller_stop_command);
  Command controller_reset_command;
  controller_reset_command.name        = "controller reset";
  controller_reset_command.description = "Reset the motor controller to ready state";
  controller_reset_command.handler     = [this, controller]() {
    core::Result result = controller->run(motors::FauxState::kReset);
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to reset the motor controller");
      return;
    }
  };
  addCommand(controller_reset_command);
  const auto optional_time_frequency_calculator
    = motors::TimeFrequencyCalculator::create(logger_, "time_frequency.json");
  if (!optional_time_frequency_calculator) {
    logger_.log(core::LogLevel::kFatal, "Failed to create time frequency calculator instance");
    return;
  }
  auto time_frequency_calculator                = std::move(*optional_time_frequency_calculator);
  const auto time_frequency_optional_controller = motors::Controller::create(
    logger_, "motor_controller_messages.json", can, time_frequency_calculator);
  if (!time_frequency_optional_controller) {
    logger_.log(core::LogLevel::kFatal, "Failed to create motor controller instance");
    return;
  }
  auto time_frequency_controller = std::move(*time_frequency_optional_controller);
  Command frequency_time_command;
  frequency_time_command.name = "controller frequency time run";
  frequency_time_command.description
    = "Run the motor controller for provided duration, with frequency pattern described in "
      "time_frequency.json";
  frequency_time_command.handler = [this, time_frequency_controller, time_frequency_calculator]() {
    std::cout << "Enter run time in seconds" << std::endl;
    std::uint32_t run_time;
    std::cin >> run_time;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    time_frequency_controller->run(motors::FauxState::kConfigure);
    time_frequency_controller->run(motors::FauxState::kReset);
    time_frequency_calculator->reset();
    std::chrono::time_point<std::chrono::system_clock> start_time_
      = std::chrono::system_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()
                                                            - start_time_)
             .count()
           < run_time) {
      core::Result result = time_frequency_controller->run(motors::FauxState::kAccelerate);
      if (result == core::Result::kFailure) {
        logger_.log(core::LogLevel::kFatal, "Failed to run the motor controller");
        return;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    time_frequency_controller->run(motors::FauxState::kStop);
  };
  addCommand(frequency_time_command);
}

void Repl::addBmsCommands(const std::string &bus)
{
  const auto optional_can = getCan(bus);
  if (!optional_can) {
    logger_.log(core::LogLevel::kFatal, "Failed to create can instance");
    return;
  }
  const auto can = std::move(*optional_can);
  const auto bms = sensors::Bms::create(logger_, can);
  Command bms_log_command;
  bms_log_command.name        = "bms log";
  bms_log_command.description = "Log the bms data";
  bms_log_command.handler     = [this, can]() {
    while (1) {
      can->receive();
    }
  };
  addCommand(bms_log_command);
}

std::optional<std::shared_ptr<io::IAdc>> Repl::getAdc(const std::uint8_t bus)
{
  const auto adc = adc_.find(bus);
  if (adc == adc_.end()) {
    const auto new_adc = io::HardwareAdc::create(logger_, bus);
    if (!new_adc) { return std::nullopt; }
    adc_.emplace(bus, *new_adc);
    return *new_adc;
  }
  return adc->second;
}

std::optional<std::shared_ptr<io::ICan>> Repl::getCan(const std::string &bus)
{
  const auto can = can_.find(bus);
  if (can == can_.end()) {
    const auto new_can = io::HardwareCan::create(logger_, bus);
    if (!new_can) { return std::nullopt; }
    can_.emplace(bus, *new_can);
    return *new_can;
  }
  return can->second;
}

std::optional<std::shared_ptr<io::II2c>> Repl::getI2c(const std::uint8_t bus)
{
  const auto i2c = i2c_.find(bus);
  if (i2c == i2c_.end()) {
    const auto new_i2c = io::HardwareI2c::create(logger_, bus);
    if (!new_i2c) { return std::nullopt; }
    i2c_.emplace(bus, *new_i2c);
    return *new_i2c;
  }
  return i2c->second;
}

std::optional<std::shared_ptr<io::Pwm>> Repl::getPwm(const io::PwmModule pwm_module,
                                                     const std::uint32_t period,
                                                     const io::Polarity polarity)
{
  const auto pwm = pwm_.find(pwm_module);
  if (pwm == pwm_.end()) {
    const auto new_pwm = io::Pwm::create(logger_, pwm_module, period, polarity);
    if (!new_pwm) { return std::nullopt; }
    pwm_.emplace(pwm_module, *new_pwm);
    return *new_pwm;
  }
  return pwm->second;
}

std::optional<std::shared_ptr<io::ISpi>> Repl::getSpi(const io::SpiBus bus,
                                                      const io::SpiMode mode,
                                                      const io::SpiWordSize word_size,
                                                      const io::SpiBitOrder bit_order,
                                                      const io::SpiClock clock)
{
  const auto spi = spi_.find(bus);
  if (spi == spi_.end()) {
    const auto new_spi = io::HardwareSpi::create(logger_, bus, mode, word_size, bit_order, clock);
    if (!new_spi) { return std::nullopt; }
    spi_.emplace(bus, *new_spi);
    return *new_spi;
  }
  return spi->second;
}

std::optional<std::shared_ptr<io::IUart>> Repl::getUart(const io::UartBus bus,
                                                        const io::UartBaudRate baud_rate,
                                                        const io::UartBitsPerByte bits_per_byte)
{
  const auto uart = uart_.find(bus);
  if (uart == uart_.end()) {
    const auto new_uart
      = io::Uart::create(logger_, static_cast<io::UartBus>(bus), baud_rate, bits_per_byte);
    if (!new_uart) { return std::nullopt; }
    uart_.emplace(bus, *new_uart);
    return *new_uart;
  }
  return uart->second;
}
}  // namespace hyped::debug
