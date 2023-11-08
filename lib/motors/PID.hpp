class PIDController {
 public:
  void PIDController_Init();
  float PIDController_Update(float setpoint, float measurement);

 private:
  // Controller Gains for tuning
  double Kp;
  double Ki;
  double Kd;

  // Derivative low-pass filter time constant
  double tau;

  // Output limits
  double limMin;
  double limMax;

  // Integrator limits
  double limMinInt;
  double limMaxInt;

  /* Sample time (in seconds) */
  double T;

  /* Controller memory */
  double integrator;
  double prevError; /* Required for integrator */
  double differentiator;
  double prevMeasurement; /* Required for differentiator */

  /* Controller output */
  double out;
};