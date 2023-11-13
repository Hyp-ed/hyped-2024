#include "core/types.hpp"
#pragma once

namespace hyped::motors {
class PidController {
 public:
  PidController();
  core::float update(core::float setpoint, core::float measurement)

    private :
      // Controller gains for tuning
      const core::float kp_;
  const core::float ki_;
  const core::float kd_;

  // Derivative low-pass filter time constant
  const core::float tau_;

  // Output limits
  const core::float minimum_output_;
  const core::float maximum_output_;

  // Integrator limits
  const core::float minimum_integrator_;
  const core::float maximum_integrator_;

  /* Sample time (in seconds) */
  const core::float sample_time_;

  /* Controller memory */
  core::float integrator_;
  core::float previous_error_; /* Required for integrator */
  core::float differentiator_;
  core::float previous_measurement_; /* Required for differentiator */
};
}  // namespace hyped::motors