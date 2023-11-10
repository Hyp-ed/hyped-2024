#include "AdcCommands.hpp"

namespace hyped::debug {
core::Result AdcCommands::addCommands(core::ILogger &logger,
                                      std::shared_ptr<Repl> repl,
                                      toml::v3::node_view<toml::v3::node> config)
{
  const auto pins = config["pins"].as_array();
  if (!pins) {
    logger.log(core::LogLevel::kFatal, "No ADC pins specified");
    return core::Result::kFailure;
  }
  for (const auto &pin : *pins) {
    const auto optional_pin = pin.value<std::uint8_t>();
    if (!optional_pin) {
      logger.log(core::LogLevel::kFatal, "Invalid ADC pin");
      return core::Result::kFailure;
    }
    const auto pin_number   = *optional_pin;
    const auto optional_adc = repl->getAdc(pin_number);
    if (!optional_adc) {
      logger.log(core::LogLevel::kFatal, "Failed to create ADC instance on pin %d", pin);
      return;
    }
    const auto adc                          = std::move(*optional_adc);
    const auto adc_read_command_name        = "adc " + std::to_string(pin_number) + " read";
    const auto adc_read_command_description = "Read from ADC pin " + std::to_string(pin_number);
    const auto adc_read_command_handler     = [&logger, adc, pin_number]() {
      const auto value = adc->readValue();
      if (!value) {
        logger.log(core::LogLevel::kFatal, "Failed to read from ADC pin %d", pin_number);
        return;
      }
      logger.log(core::LogLevel::kDebug, "ADC value from pin %d: %f", pin_number, *value);
    };
    auto adc_read_command = std::make_unique<Command>(
      adc_read_command_name, adc_read_command_description, adc_read_command_handler);
    repl->addCommand(std::move(adc_read_command));
  }
  return core::Result::kSuccess;
}
}  // namespace hyped::debug