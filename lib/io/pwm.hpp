#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include <core/logger.hpp>
#include <core/types.hpp>

namespace hyped::io {

enum class PwmModule {
  kECapPwm0 = 0,
  kECapPwm2,
  kEHRPwm0A,
  kEHRPwm0B,
  kEHRPwm1A,
  kEHRPwm1B,
  kEHRPwm2A,
  kEHRPwm2B
};
enum class Polarity { kActiveHigh = 0, kActiveLow };
enum class Mode { kStop = 0, kRun };

// use this class if a high‐frequency periodic switching signal is required
// PWM can achieve frequencies of 1 MHz or higher, without a significant CPU load
// PWM can have a variable duty cycle but the period and polarity is fixed
class Pwm {
 public:
  /**
   * @brief Creates a PWM object and gets all the relevant file descriptors to do I/O operations
   */
  static std::optional<std::shared_ptr<Pwm>> create(core::ILogger &logger,
                                                    const PwmModule pwm_module,
                                                    const std::uint32_t period,
                                                    const Polarity polarity);
  Pwm(core::ILogger &logger,
      const std::uint32_t period,
      const int period_file,
      const int duty_cycle_file,
      const int enable_file);
  ~Pwm();

  /**
   * @brief Set the mode of the PWM signal
   * @param mode the mode of the PWM signal
   * Mode is either stop or run
   * @return kSuccess if the mode was set successfully
   */
  core::Result setMode(const Mode mode);

  /**
   * @brief Set the duty cycle of the PWM signal using a percentage
   * @param duty_cycle the duty cycle of the PWM signal
   * The valid values for duty cycle are 0.0 to 1.0 (0% to 100%)
   * @return kSuccess if the duty cycle was set successfully
   */
  core::Result setDutyCycleByPercentage(const core::Float duty_cycle);

 private:
  /**
   * @brief Set the duty cycle of the PWM signal using a value for active time
   * @param time_active the length of time the PWM signal is "active" in ns
   * The valid values for time active are 0 to the period (0% to 100%)
   * @return kSuccess if the duty cycle was set successfully
   */
  core::Result setDutyCycleByTime(const std::uint32_t time_active);

  /**
   * @brief Set the period of the PWM signal
   * @param period the period of the PWM signal in ns
   * @return kSuccess if the period was set successfully
   */
  static core::Result setPeriod(const std::uint32_t period, const int period_file);

  /**
   * @brief Set the polarity of the PWM signal
   * @param polarity the polarity of the PWM signal
   * Polarity is either active high or active low
   * @return kSuccess if the polarity was set successfully
   */
  static core::Result setPolarity(const Polarity polarity, const int polarity_file);

  /**
   * @brief Get the corect folder name for the chosen PWM module
   * @param pwm_module the PWM module to get the folder name for
   * @return the folder name
   */
  static std::string getPwmFolderName(const PwmModule pwm_module);

 private:
  core::ILogger &logger_;
  core::Float current_duty_cycle_;
  std::uint32_t current_time_active_;  // ns
  std::uint32_t current_period_;       // ns
  Mode current_mode_;
  Polarity current_polarity_;
  int duty_cycle_file_;
  int enable_file_;
};

}  // namespace hyped::io
