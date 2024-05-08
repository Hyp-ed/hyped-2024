#include "pwm_commands.hpp"

namespace hyped::debug {

core::Result PwmCommands::addCommands(core::ILogger &logger, std::shared_ptr<Repl> &repl)
{
  {
    const auto *const pwm_run_command_name        = "pwm run";
    const auto *const pwm_run_command_description = "Run a PWM module";
    const auto *const pwm_run_command_usage       = "pwm run <module> <period> <duty_cycle>";
    const auto pwm_run_command_handler            = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 3) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto module = static_cast<io::PwmModule>(std::stoi(args[0]));
      const auto period = std::stoi(args[1]);
      auto optional_pwm = repl->getPwm(module, period, io::Polarity::kActiveHigh);
      if (!optional_pwm) {
        logger.log(core::LogLevel::kFatal, "Failed to get PWM module");
        return;
      }
      const auto pwm                           = std::move(*optional_pwm);
      core::Float duty_cycle                   = std::stof(args[2]);
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
    auto pwm_run_command = std::make_unique<Command>(pwm_run_command_name,
                                                     pwm_run_command_description,
                                                     pwm_run_command_usage,
                                                     pwm_run_command_handler);
    repl->addCommand(std::move(pwm_run_command));
  }
  {
    const auto *const pwm_stop_command_name        = "pwm stop";
    const auto *const pwm_stop_command_description = "Stop a PWM module";
    const auto *const pwm_stop_command_usage       = "pwm stop <module>";
    const auto pwm_stop_command_handler = [&logger, repl](std::vector<std::string> args) {
      if (args.size() != 1) {
        logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
        return;
      }
      const auto module = static_cast<io::PwmModule>(std::stoi(args[0]));
      auto optional_pwm = repl->getPwm(module, 0, io::Polarity::kActiveHigh);
      if (!optional_pwm) {
        logger.log(core::LogLevel::kFatal, "Failed to get PWM module");
        return;
      }
      const auto pwm                    = std::move(*optional_pwm);
      const core::Result disable_result = pwm->setMode(io::Mode::kStop);
      if (disable_result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to stop PWM module");
        return;
      }
    };
    auto pwm_stop_command = std::make_unique<Command>(pwm_stop_command_name,
                                                      pwm_stop_command_description,
                                                      pwm_stop_command_usage,
                                                      pwm_stop_command_handler);
    repl->addCommand(std::move(pwm_stop_command));
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::debug
