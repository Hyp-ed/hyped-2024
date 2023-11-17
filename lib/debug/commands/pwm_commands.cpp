#include "pwm_commands.hpp"

namespace hyped::debug {

core::Result PwmCommands::addCommands(core::ILogger &logger,
                                      std::shared_ptr<Repl> repl,
                                      toml::v3::node_view<toml::v3::node> config)
{
  const auto modules = config["modules"].as_array();
  if (!modules) {
    logger.log(core::LogLevel::kFatal, "No PWM modules specified");
    return core::Result::kFailure;
  }
  const auto period = config["period"].as_array();
  if (!period) {
    logger.log(core::LogLevel::kFatal, "No PWM period specified");
    return core::Result::kFailure;
  }
  if (modules->size() != period->size()) {
    logger.log(core::LogLevel::kFatal, "PWM modules and period size mismatch");
    return core::Result::kFailure;
  }
  for (std::uint8_t i = 0; i < modules->size(); i++) {
    const auto optional_module = modules->at(i).value<std::uint8_t>();
    if (!optional_module) {
      logger.log(core::LogLevel::kFatal, "Invalid PWM module");
      return core::Result::kFailure;
    }
    const auto module          = static_cast<io::PwmModule>(*optional_module);
    const auto optional_period = period->at(i).value<std::uint32_t>();
    if (!optional_period) {
      logger.log(core::LogLevel::kFatal, "Invalid PWM period");
      return core::Result::kFailure;
    }
    const auto period       = *optional_period;
    const auto optional_pwm = repl->getPwm(module, period, io::Polarity::kActiveHigh);
    if (!optional_pwm) {
      logger.log(core::LogLevel::kFatal, "Failed to create PWM module");
      return core::Result::kFailure;
    }
    const auto pwm = std::move(*optional_pwm);
    {
      const auto pwm_run_command_name = "pwm " + std::to_string(static_cast<int>(module)) + " run";
      const auto pwm_run_command_description
        = "Run PWM module " + std::to_string(static_cast<int>(module));
      const auto pwm_run_command_handler = [&logger, pwm, module]() {
        core::Float duty_cycle;
        std::cout << "Duty cycle: ";
        std::cin >> duty_cycle;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        const core::Result duty_cycle_set_result = pwm->setDutyCycleByPercentage(duty_cycle);
        if (duty_cycle_set_result == core::Result::kFailure) {
          logger.log(core::LogLevel::kFatal, "Failed to set PWM duty cycle");
          return;
        }
        const core::Result enable_result = pwm->setMode(io::Mode::kRun);
        if (enable_result == core::Result::kFailure) {
          logger.log(core::LogLevel::kFatal, "Failed to enable PWM module");
          return;
        }
      };
      auto pwm_run_command = std::make_unique<Command>(
        pwm_run_command_name, pwm_run_command_description, pwm_run_command_handler);
      repl->addCommand(std::move(pwm_run_command));
    }
    {
      const auto pwm_stop_command_name
        = "pwm " + std::to_string(static_cast<int>(module)) + " stop";
      const auto pwm_stop_command_description
        = "Stop PWM module " + std::to_string(static_cast<int>(module));
      const auto pwm_stop_command_handler = [&logger, pwm, module]() {
        const core::Result disable_result = pwm->setMode(io::Mode::kStop);
        if (disable_result == core::Result::kFailure) {
          logger.log(core::LogLevel::kFatal, "Failed to stop PWM module");
          return;
        }
      };
      auto pwm_stop_command = std::make_unique<Command>(
        pwm_stop_command_name, pwm_stop_command_description, pwm_stop_command_handler);
      repl->addCommand(std::move(pwm_stop_command));
    }
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::debug
