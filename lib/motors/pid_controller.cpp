#include "pid_controller.hpp"

namespace hyped::motors {
PidController::PidController(

  /* Clear controller variables */
  const core::float kp,
  const core::float ki,
  const core::float kd,
  const core::float minimum_output,
  const core::float maximum_output,
  const core::float minimum_integrator,
  const core::float maximum_integrator,

  const core::float output = 0.0f;)
    : integrator_{0},
      previous_error_{0},
      differentiator_{0},
      previous_measurement_{0},
      kp_(kp),
      ki_(ki),
      kd_(kd),
      minimum_output_(minimum_output),
      maximum_output_(maximum_output),
      minimum_integrator_(minimum_integrator),
      maximum_integrator_(maximum_integrator);
{
}

core::float PidController::update(core::float setpoint, core::float measurement)
{
  const core::float error = setpoint - measurement;

  const core::float proportional = kp_ * error;

  integrator_ = integrator_ + 0.5f * ki_ * sample_time_ * (error + previous_error_);

  // Integrator anti wind-up (dynamic integrator clamping)
  if (integrator_ > maximum_integrator_) {
    integrator_ = maximum_integrator_;

  } else if (integrator_ < minimum_integrator_) {
    integrator_ = minimum_integrator_;
  }

  // Derivative term (optional) with low pass filter
  differentiator_
    // Derivative on measurement, hence minus sign in front of equation
    = -(2.0f * kd_ * (measurement - previous_measurement_)
        + (2.0f * tau_ - sample_time_) * differentiator_)
      / (2.0f * tau_ + sample_time_);

  // Store error and measurement for later use
  previous_error_       = error;
  previous_measurement_ = measurement;

  // Set controller output and limit outputs if needed
  const core::float output = proportional + integrator_ + differentiator_;

  if (output > maximum_output_) {
    output = maximum_output_;

  } else if (output < minimum_output_) {
    output = minimum_output_;
  }

  // Return controller output
  return output;
};
}  // namespace hyped::motors