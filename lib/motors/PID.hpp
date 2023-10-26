struct PIDController {


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
	double prevError;			/* Required for integrator */
	double differentiator;
	double prevMeasurement;		/* Required for differentiator */

	/* Controller output */
	double out;
} ;

void  PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid, float setpoint, float measurement);