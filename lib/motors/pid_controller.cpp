#include "pid_controller.hpp"

namespace hyped::motors {
PidController::PidController(const core::Float kp,
                             const core::Float ki,
                             const core::Float kd,
                             const core::Float tau,
                             const core::Float minimum_output,
                             const core::Float maximum_output,
                             const core::Float minimum_integrator,
                             const core::Float maximum_integrator,
                             const core::Float sample_time)
    : integrator_(0),
      previous_error_(0),
      differentiator_(0),
      previous_measurement_(0),
      kp_(kp),
      ki_(ki),
      kd_(kd),
      tau_(tau),
      minimum_output_(minimum_output),
      maximum_output_(maximum_output),
      minimum_integrator_(minimum_integrator),
      maximum_integrator_(maximum_integrator),
      sample_time_(sample_time)
{
}

core::Float PidController::update(core::Float setpoint, core::Float measurement)
{
  const core::Float error        = setpoint - measurement;
  const core::Float proportional = kp_ * error;
  integrator_ = integrator_ + 0.5 * ki_ * sample_time_ * (error + previous_error_);
  // Integrator anti wind-up (dynamic integrator clamping)
  if (integrator_ > maximum_integrator_) {
    integrator_ = maximum_integrator_;
  } else if (integrator_ < minimum_integrator_) {
    integrator_ = minimum_integrator_;
  }
  // Derivative term (optional and on measurement, hence minus sign) with low pass filter
  differentiator_ = -(2.0 * kd_ * (measurement - previous_measurement_)
                      + (2.0 * tau_ - sample_time_) * differentiator_)
                    / (2.0 * tau_ + sample_time_);
  previous_error_       = error;
  previous_measurement_ = measurement;
  core::Float output    = proportional + integrator_ + differentiator_;

  if (output > maximum_output_) {
    output = maximum_output_;
  } else if (output < minimum_output_) {
    output = minimum_output_;
  }
  return output;
}
}  // namespace hyped::motors