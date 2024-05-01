#include "repl.hpp"

#include "commands/adc_commands.hpp"
#include "commands/can_commands.hpp"
#include "commands/gpio_commands.hpp"
#include "commands/i2c_commands.hpp"
#include "commands/pwm_commands.hpp"
#include "commands/spi_commands.hpp"
#include "commands/uart_commands.hpp"
#include <core/wall_clock.hpp>
#include <io/hardware_adc.hpp>
#include <io/hardware_can.hpp>
#include <io/hardware_i2c.hpp>

namespace hyped::debug {

// NOLINTBEGIN(readability-function-cognitive-complexity)
std::optional<std::shared_ptr<Repl>> Repl::create(core::ILogger &logger,
                                                  Terminal &terminal,
                                                  const std::string &filename)
{
  auto repl = std::make_shared<Repl>(logger, terminal);
  toml::table config;
  try {
    config = toml::parse_file(filename);
  } catch (const toml::parse_error &e) {
    logger.log(core::LogLevel::kFatal, "Error parsing TOML file: %s", e.description());
    return std::nullopt;
  }
  if (config["io"]["adc"]["enabled"].value_or(false)) {
    const auto result = AdcCommands::addCommands(logger, repl);
    if (result == core::Result::kFailure) {
      logger.log(core::LogLevel::kFatal, "Error adding ADC commands");
      return std::nullopt;
    }
  }
  if (config["io"]["can"]["enabled"].value_or(false)) {
    const auto result = CanCommands::addCommands(logger, repl);
    if (result == core::Result::kFailure) {
      logger.log(core::LogLevel::kFatal, "Error adding CAN commands");
      return std::nullopt;
    }
  }
  if (config["io"]["gpio"]["enabled"].value_or(false)) {
    const auto result = GpioCommands::addCommands(logger, repl);
    if (result == core::Result::kFailure) {
      logger.log(core::LogLevel::kFatal, "Error adding GPIO commands");
      return std::nullopt;
    }
  }
  if (config["io"]["i2c"]["enabled"].value_or(false)) {
    const auto result = I2cCommands::addCommands(logger, repl);
    if (result == core::Result::kFailure) {
      logger.log(core::LogLevel::kFatal, "Error adding I2C commands");
      return std::nullopt;
    }
  }
  if (config["io"]["pwm"]["enabled"].value_or(false)) {
    const auto result = PwmCommands::addCommands(logger, repl);
    if (result == core::Result::kFailure) {
      logger.log(core::LogLevel::kFatal, "Error adding PWM commands");
      return std::nullopt;
    }
  }
  if (config["io"]["spi"]["enabled"].value_or(false)) {
    const auto result = SpiCommands::addCommands(logger, repl);
    if (result == core::Result::kFailure) {
      logger.log(core::LogLevel::kFatal, "Error adding SPI commands");
      return std::nullopt;
    }
  }
  if (config["io"]["uart"]["enabled"].value_or(false)) {
    const auto result = UartCommands::addCommands(logger, repl);
    if (result == core::Result::kFailure) {
      logger.log(core::LogLevel::kFatal, "Error adding UART commands");
      return std::nullopt;
    }
  }
  auto *const aliases = config["aliases"].as_table();
  for (auto [alias, command] : *aliases) {
    const std::string alias_alias     = static_cast<std::string>(alias.str());
    const auto optional_alias_command = command.value<std::string>();
    if (!optional_alias_command) {
      logger.log(core::LogLevel::kFatal, "Error parsing alias command: %s", alias_alias.c_str());
      return std::nullopt;
    }
    const std::string &alias_command = *optional_alias_command;
    const auto result                = repl->addAlias(alias_alias, alias_command);
    if (result == core::Result::kFailure) { return std::nullopt; }
  }
  return repl;
}
// NOLINTEND(readability-function-cognitive-complexity)

Repl::Repl(core::ILogger &logger, Terminal &terminal)
    : logger_(logger),
      terminal_(terminal),
      gpio_(std::make_shared<io::HardwareGpio>(logger))
{
  addHelpCommand();
  addQuitCommand();
}

// NOLINTBEGIN(readability-function-cognitive-complexity) TODO refactor this
void Repl::run()
{
  int i = 0;
  std::string input;

  while (true) {
    terminal_.refresh_line(input, ">> ");
    const auto &[key, letter] = terminal_.prompt();
    if (key == debug::KeyPress::kASCII) {
      input += letter;
    } else if (key == debug::KeyPress::kEnter) {
      terminal_.cr();
      history_.push_back(input);
      const auto alias = aliases_.find(input);
      if (alias != aliases_.end()) { input = alias->second; }
      for (auto &command : commands_) {
        // Match on first command that is a prefix of the input
        if (input.starts_with(command->getName())) {
          std::vector<std::string> args;
          // Get argument string and remove command from it
          std::stringstream ss(input.substr(command->getName().size()));
          std::string arg;
          while (getline(ss, arg, ' ')) {
            // Discard whitespace
            if (arg.empty()) { continue; }
            args.push_back(arg);
          }
          command->execute(args);
          break;
        }
      }
      input = "";
      i     = 0;
    } else if (key == debug::KeyPress::kUp) {
      if (i < history_.size()) {
        input = history_[history_.size() - 1 - i];
        i++;
      }
    } else if (key == debug::KeyPress::kDown) {
      if (i > 0) {
        i--;
        input = history_[history_.size() - 1 - i];
      }
    } else if (key == debug::KeyPress::kBackspace) {
      if (input.empty()) { input.pop_back(); }
    } else if (key == debug::KeyPress::kTab) {
      std::vector<std::string> matches = autoComplete(input);
      if (matches.size() == 1) {
        input = matches[0];
      } else if (matches.size() > 1) {
        terminal_.cr();
        for (auto &match : matches) {
          terminal_.printf("%s\n", match.c_str());
        }
        terminal_.refresh_line(input, ">> ");
      }
    } else if (key == debug::KeyPress::kEscape) {
      input = "";
      terminal_.cr();
    }
  }
}
// NOLINTEND(readability-function-cognitive-complexity)

std::vector<std::string> Repl::autoComplete(const std::string &partial)
{
  std::vector<std::string> matches;
  for (auto &command : commands_) {
    if (command->getName().starts_with(partial)) { matches.push_back(command->getName()); }
  }
  for (auto &[alias, command] : aliases_) {
    if (alias.starts_with(partial)) { matches.push_back(alias); }
  }
  return matches;
}

void Repl::addCommand(std::unique_ptr<Command> command)
{
  logger_.log(core::LogLevel::kDebug, "Adding command: %s", command->getName().c_str());
  commands_.push_back(std::move(command));
}

core::Result Repl::addAlias(const std::string &alias, const std::string &alias_command)
{
  logger_.log(
    core::LogLevel::kDebug, "Adding alias: %s -> %s", alias.c_str(), alias_command.c_str());
  for (auto &command : commands_) {
    const auto command_name = command->getName();
    if (command_name == alias_command) {
      aliases_.emplace(alias, command_name);
      return core::Result::kSuccess;
    }
  }
  logger_.log(core::LogLevel::kFatal, "Alias command not found: %s", alias_command.c_str());
  return core::Result::kFailure;
}

void Repl::printHelp()
{
  for (auto &command : commands_) {
    logger_.log(core::LogLevel::kDebug,
                "%s: %s, usage: '%s'",
                command->getName().c_str(),
                command->getDescription().c_str(),
                command->getUsage().c_str());
  }
}

void Repl::addHelpCommand()
{
  addCommand(std::make_unique<Command>(
    "help", "Print this help message", "help", [this](const std::vector<std::string> &args) {
      printHelp();
      return;
    }));
}

void Repl::addQuitCommand()
{
  addCommand(std::make_unique<Command>(
    "quit", "Quit the program", "quit", [this](const std::vector<std::string> &args) {
      terminal_.quit();
      exit(0);
    }));
}

std::optional<std::shared_ptr<io::IAdc>> Repl::getAdc(const std::uint8_t pin)
{
  const auto adc = adc_.find(pin);
  if (adc == adc_.end()) {
    const auto new_adc = io::HardwareAdc::create(logger_, pin);
    if (!new_adc) { return std::nullopt; }
    adc_.emplace(pin, *new_adc);
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
