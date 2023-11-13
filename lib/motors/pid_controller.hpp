#include "core/types.hpp"
#pragma once

namespace hyped::motors {
class PidController {
 public:
  PidController(

    const core::Float kp,
    const core::Float ki,
    const core::Float kd,
    const core::Float tau,
    const core::Float minimum_output,
    const core::Float maximum_output,
    const core::Float minimum_integrator,
    const core::Float maximum_integrator,
    const core::Float sample_time);

  core::Float update(core::Float setpoint, core::Float measurement);

 private:
  // Controller gains for tuning
  const core::Float kp_;
  const core::Float ki_;
  const core::Float kd_;

  // Derivative low-pass filter time constant
  const core::Float tau_;

  // Output limits
  const core::Float minimum_output_;
  const core::Float maximum_output_;

  // Integrator limits
  const core::Float minimum_integrator_;
  const core::Float maximum_integrator_;

  /* Sample time (in seconds) */
  const core::Float sample_time_;

  /* Controller memory */
  core::Float integrator_;
  core::Float previous_error_; /* Required for integrator */
  core::Float differentiator_;
  core::Float previous_measurement_; /* Required for differentiator */
};
}  // namespace hyped::motors