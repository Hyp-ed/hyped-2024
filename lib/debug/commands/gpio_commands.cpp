#include "gpio_commands.hpp"

namespace hyped::debug {

core::Result GpioCommands::addCommands(core::ILogger &logger, std::shared_ptr<Repl> repl)
{
  {
    const auto gpio_read_command_name        = "gpio read";
    const auto gpio_read_command_description = "Read from GPIO pins";
    const auto gpio_read_command_usage       = "gpio read <pin_number>";
    const auto gpio_read_command_handler     = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 1) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto gpio                 = repl->getGpio();
      const auto pin                  = std::stoi(args[0]);
      const auto optional_gpio_reader = gpio->getReader(pin, io::Edge::kNone);
      if (!optional_gpio_reader) {
        logger.log(core::LogLevel::kFatal, "Failed to get GPIO reader on pin %d", pin);
        return;
      }
      const auto gpio_reader = std::move(*optional_gpio_reader);
      const auto value       = gpio_reader->read();
      if (!value) {
        logger.log(core::LogLevel::kFatal, "Failed to read from GPIO pin %d", pin);
        return;
      }
      logger.log(core::LogLevel::kDebug,
                 "GPIO value from pin %d: %d",
                 pin,
                 static_cast<std::uint8_t>(*value));
    };
    auto gpio_read_command = std::make_unique<Command>(gpio_read_command_name,
                                                       gpio_read_command_description,
                                                       gpio_read_command_usage,
                                                       gpio_read_command_handler);
    repl->addCommand(std::move(gpio_read_command));
  }
  {
    const auto gpio_write_command_name        = "gpio write";
    const auto gpio_write_command_description = "Write to GPIO pin (0 or 1)";
    const auto gpio_write_command_usage       = "gpio write <pin_number> <value>";
    const auto gpio_write_command_handler     = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 2) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto gpio                 = repl->getGpio();
      const auto pin                  = std::stoi(args[0]);
      const auto optional_gpio_writer = gpio->getWriter(pin, io::Edge::kNone);
      if (!optional_gpio_writer) {
        logger.log(core::LogLevel::kFatal, "Failed to create GPIO writer on pin %d", pin);
        return;
      }
      const auto gpio_writer = std::move(*optional_gpio_writer);
      const auto value       = std::stoi(args[1]);
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
          return;
      }
      const auto result = gpio_writer->write(signal);
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to write to GPIO pin %d", pin);
        return;
      }
      logger.log(core::LogLevel::kDebug, "Wrote %d to GPIO pin %d", signal, pin);
    };
    auto gpio_write_command = std::make_unique<Command>(gpio_write_command_name,
                                                        gpio_write_command_description,
                                                        gpio_write_command_usage,
                                                        gpio_write_command_handler);
    repl->addCommand(std::move(gpio_write_command));
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::debug