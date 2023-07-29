#include "pwm.hpp"

#include <fcntl.h>
#include <unistd.h>

namespace hyped::io {

std::optional<std::shared_ptr<Pwm>> Pwm::create(core::ILogger &logger,
                                                const PwmModule pwm_module,
                                                const std::uint32_t period,
                                                const Polarity polarity)
{
  const std::string pwm_address    = "/sys/class/pwm/" + getPwmFolderName(pwm_module) + "/";
  const std::string period_address = pwm_address + "period";
  const int period_file            = open(period_address.c_str(), O_WRONLY);
  if (period_file < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to open PWM period file");
    return std::nullopt;
  }
  const core::Result period_result = setPeriod(period, period_file);
  if (period_result == core::Result::kFailure) {
    logger.log(core::LogLevel::kFatal, "Failed to set PWM period");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully set PWM period to %d", period);
  const std::string polarity_address = pwm_address + "polarity";
  const int polarity_file            = open(polarity_address.c_str(), O_WRONLY);
  if (polarity_file < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to open PWM polarity file");
    return std::nullopt;
  }
  const core::Result polarity_result = setPolarity(polarity, polarity_file);
  if (polarity_result == core::Result::kFailure) {
    logger.log(core::LogLevel::kFatal, "Failed to set PWM polarity");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully set PWM polarity to %d", polarity);
  const std::string enable_address = pwm_address + "enable";
  const int enable_file            = open(enable_address.c_str(), O_WRONLY);
  if (enable_file < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to open PWM enable file");
    return std::nullopt;
  }
  const std::string duty_cycle_address = pwm_address + "duty_cycle";
  const int duty_cycle_file            = open(duty_cycle_address.c_str(), O_WRONLY);
  if (duty_cycle_file < 0) {
    logger.log(core::LogLevel::kFatal, "Failed to open PWM duty cycle file");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully initialised PWM");
  return std::make_shared<Pwm>(logger, period, period_file, duty_cycle_file, enable_file);
}

Pwm::Pwm(core::ILogger &logger,
         const std::uint32_t period,
         const int period_file,
         const int duty_cycle_file,
         const int enable_file)
    : logger_(logger),
      current_period_(period),
      current_duty_cycle_(0),
      current_time_active_(0),
      current_mode_(Mode::kStop),
      duty_cycle_file_(duty_cycle_file),
      enable_file_(enable_file)
{
}

Pwm::~Pwm()
{
  close(duty_cycle_file_);
  close(enable_file_);
}

core::Result Pwm::setMode(const Mode mode)
{
  // Avoid I/O if no change is required
  if (current_mode_ == mode) {
    logger_.log(core::LogLevel::kDebug, "PWM mode is already set to %d, skipping I/O", mode);
    return core::Result::kSuccess;
  }
  // Duty cycle cannot be 0 when running
  if (current_duty_cycle_ == 0 && mode == Mode::kRun) {
    logger_.log(core::LogLevel::kFatal, "Failed to run PWM, duty cycle is 0");
    return core::Result::kFailure;
  }
  const std::uint8_t mode_value = static_cast<std::uint8_t>(mode);
  char write_buffer[2];
  snprintf(write_buffer, sizeof(write_buffer), "%d", mode_value);
  const ssize_t num_bytes_written = write(enable_file_, write_buffer, sizeof(write_buffer));
  if (num_bytes_written != sizeof(write_buffer)) {
    logger_.log(core::LogLevel::kFatal, "Failed to write to PWM enable file");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully set PWM mode to %d", mode_value);
  current_mode_ = mode;
  return core::Result::kSuccess;
}

core::Result Pwm::setDutyCycleByPercentage(const core::Float duty_cycle)
{
  // Duty cycle cannot be set when running
  if (current_mode_ == Mode::kRun) {
    logger_.log(core::LogLevel::kFatal, "Failed to set duty cycle, PWM is running");
    return core::Result::kFailure;
  }
  if (duty_cycle > 1.0) {
    logger_.log(core::LogLevel::kFatal,
                "Failed to set duty cycle, percentage cannot be greater than 1.0");
    return core::Result::kFailure;
  }
  if (duty_cycle <= 0.0) {
    logger_.log(core::LogLevel::kFatal,
                "Failed to set duty cycle, percentage cannot be less than or equal to 0.0");
    return core::Result::kFailure;
  }
  const std::uint32_t time_active = static_cast<std::uint32_t>(duty_cycle * current_period_);
  current_duty_cycle_             = duty_cycle;
  return setDutyCycleByTime(time_active);
}

core::Result Pwm::setDutyCycleByTime(const std::uint32_t time_active)
{
  // Avoid I/O if no change is required
  if (time_active == current_time_active_) {
    logger_.log(
      core::LogLevel::kDebug, "Duty cycle is already set to %d, skipping I/O", time_active);
    return core::Result::kSuccess;
  }
  char write_buffer[10];
  snprintf(write_buffer, sizeof(write_buffer), "%d", time_active);
  const ssize_t num_bytes_written = write(duty_cycle_file_, write_buffer, sizeof(write_buffer));
  if (num_bytes_written != sizeof(write_buffer)) {
    logger_.log(core::LogLevel::kFatal, "Failed to write to PWM duty cycle file");
    return core::Result::kFailure;
  }
  logger_.log(core::LogLevel::kDebug, "Successfully set PWM active time to %d", time_active);
  current_time_active_ = time_active;
  return core::Result::kSuccess;
}

core::Result Pwm::setPeriod(const std::uint32_t period, const int period_file)
{
  char write_buffer[10];
  snprintf(write_buffer, sizeof(write_buffer), "%d", period);
  const ssize_t num_bytes_written = write(period_file, write_buffer, sizeof(write_buffer));
  if (num_bytes_written != sizeof(write_buffer)) { return core::Result::kFailure; }
  close(period_file);
  return core::Result::kSuccess;
}

core::Result Pwm::setPolarity(const Polarity polarity, const int polarity_file)
{
  const std::uint8_t polarity_value = static_cast<std::uint8_t>(polarity);
  char write_buffer[7];
  if (polarity_value == 0) {
    snprintf(write_buffer, sizeof(write_buffer), "normal");
  } else {
    snprintf(write_buffer, sizeof(write_buffer), "inversed");
  }
  const ssize_t num_bytes_written = write(polarity_file, write_buffer, sizeof(write_buffer));
  if (num_bytes_written != sizeof(write_buffer)) { return core::Result::kFailure; }
  close(polarity_file);
  return core::Result::kSuccess;
}

std::string Pwm::getPwmFolderName(const PwmModule pwm_module)
{
  switch (pwm_module) {
    case PwmModule::kECapPwm0:
      return "pwm-0:0";
    case PwmModule::kECapPwm2:
      // TODOLater: Check this particular case in TLD
      return "pwm-6:0";
    case PwmModule::kEHRPwm0A:
      return "pwm-1:0";
    case PwmModule::kEHRPwm0B:
      return "pwm-1:1";
    case PwmModule::kEHRPwm1A:
      return "pwm-4:0";
    case PwmModule::kEHRPwm1B:
      return "pwm-4:1";
    case PwmModule::kEHRPwm2A:
      return "pwm-7:0";
    case PwmModule::kEHRPwm2B:
      return "pwm-7:1";
    default:  // for compiler
      return "";
  }
}

}  // namespace hyped::io