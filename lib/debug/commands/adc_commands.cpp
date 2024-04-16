#include "adc_commands.hpp"

namespace hyped::debug {

core::Result AdcCommands::addCommands(core::ILogger &logger,
                                      std::shared_ptr<Repl> repl,
                                      toml::v3::node_view<toml::v3::node> config)
{
  const auto adc_read_command_name        = "adc read";
  const auto adc_read_command_description = "Read from ADC pin";
  const auto adc_read_command_usage       = "adc read <pin_number>";
  const auto adc_read_command_handler     = [&logger, repl](std::vector<std::string> args) {
    if (args.size() != 1) {
      logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
      return;
    }
    const auto pin_number   = std::stoi(args[0]);
    const auto optional_adc = repl->getAdc(pin_number);
    if (!optional_adc) {
      logger.log(core::LogLevel::kFatal, "Failed to get ADC instance on pin %d", pin_number);
      return;
    }
    const auto adc   = std::move(*optional_adc);
    const auto value = adc->readValue();
    if (!value) {
      logger.log(core::LogLevel::kFatal, "Failed to read from ADC pin %d", pin_number);
      return;
    }
    logger.log(core::LogLevel::kDebug, "ADC value from pin %d: %f", pin_number, *value);
  };
  auto adc_read_command = std::make_unique<Command>(adc_read_command_name,
                                                    adc_read_command_description,
                                                    adc_read_command_usage,
                                                    adc_read_command_handler);
  repl->addCommand(std::move(adc_read_command));
  return core::Result::kSuccess;
}

}  // namespace hyped::debug