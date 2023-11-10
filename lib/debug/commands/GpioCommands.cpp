#include "GpioCommands.hpp"

namespace hyped::debug {
core::Result GpioCommands::addCommands(core::ILogger &logger,
                                       std::shared_ptr<Repl> repl,
                                       toml::v3::node_view<toml::v3::node> config)
{
  const auto read_pins = config["read_pins"].as_array();
  const auto gpio      = repl->getGpio();
  for (const auto &read_pin : *read_pins) {
    const auto optional_read_pin = read_pin.value<std::uint8_t>();
    if (!optional_read_pin) {
      logger.log(core::LogLevel::kFatal, "Invalid GPIO read pin");
      return core::Result::kFailure;
    }
    const auto pin                  = *optional_read_pin;
    const auto optional_gpio_reader = gpio->getReader(pin, io::Edge::kNone);
    if (!optional_gpio_reader) {
      logger.log(core::LogLevel::kFatal, "Failed to create GPIO reader on pin %d", pin);
      return core::Result::kFailure;
    }
    const auto gpio_reader                   = std::move(*optional_gpio_reader);
    const auto gpio_read_command_name        = "gpio " + std::to_string(pin) + " read";
    const auto gpio_read_command_description = "Read from GPIO pin " + std::to_string(pin);
    const auto gpio_read_command_handler     = [&logger, gpio_reader, pin]() {
      const auto value = gpio_reader->read();
      if (!value) {
        logger.log(core::LogLevel::kFatal, "Failed to read from GPIO pin %d", pin);
        return;
      }
      logger.log(core::LogLevel::kDebug,
                 "GPIO value from pin %d: %d",
                 pin,
                 static_cast<std::uint8_t>(*value));
    };
    auto gpio_read_command = std::make_unique<Command>(
      gpio_read_command_name, gpio_read_command_description, gpio_read_command_handler);
    repl->addCommand(std::move(gpio_read_command));
  }
  const auto write_pins = config["write_pins"].as_array();
  for (const auto &write_pin : *write_pins) {
    const auto optional_write_pin = write_pin.value<std::uint8_t>();
    if (!optional_write_pin) {
      logger.log(core::LogLevel::kFatal, "Invalid GPIO write pin");
      return core::Result::kFailure;
    }
    const auto pin                  = *optional_write_pin;
    const auto optional_gpio_writer = gpio->getWriter(pin, io::Edge::kNone);
    if (!optional_gpio_writer) {
      logger.log(core::LogLevel::kFatal, "Failed to create GPIO writer on pin %d", pin);
      return core::Result::kFailure;
    }
    const auto gpio_writer             = std::move(*optional_gpio_writer);
    const auto gpio_write_command_name = "gpio " + std::to_string(pin) + " write";
    const auto gpio_write_command_description
      = "Write to GPIO pin " + std::to_string(pin) + " (0 or 1)";
    const auto gpio_write_command_handler = [&logger, gpio_writer, pin]() {
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
          logger.log(core::LogLevel::kFatal, "Invalid GPIO value: %d, must be 0 or 1", value);
          break;
      }
      const auto result = gpio_writer->write(signal);
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to write to GPIO pin %d", pin);
        return;
      }
      logger.log(core::LogLevel::kDebug, "Wrote %d to GPIO pin %d", value, pin);
    };
    auto gpio_write_command = std::make_unique<Command>(
      gpio_write_command_name, gpio_write_command_description, gpio_write_command_handler);
    repl->addCommand(std::move(gpio_write_command));
  }
  return core::Result::kSuccess;
}
}  // namespace hyped::debug