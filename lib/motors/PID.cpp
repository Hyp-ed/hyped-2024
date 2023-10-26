#include "PID.hpp"


void PIDController_Init(PIDController *pid) {
    	
    /* Clear controller variables */
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;
	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;

};

float PIDController_Update(PIDController *pid, float setpoint, float measurement) {

	// Error signal 
    float error = setpoint - measurement;

	// Proportional term
    float proportional = pid->Kp * error;

	// Integral term
	pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

	// Integrator Anti wind-up (dynamic integrator clamping)
    if (pid->integrator > pid->limMaxInt) {

        pid->integrator = pid->limMaxInt;

    } else if (pid->integrator < pid->limMinInt) {

        pid->integrator = pid->limMinInt;

    }


	// Derivative Term (optional) with low pass filter		
    pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);



	// Set controller output and limit outputs if needed
    pid->out = proportional + pid->integrator + pid->differentiator;

    if (pid->out > pid->limMax) {

        pid->out = pid->limMax;

    } else if (pid->out < pid->limMin) {

        pid->out = pid->limMin;

    }

	// Store error and measurement for later use 
    pid->prevError       = error;
    pid->prevMeasurement = measurement;

	// Return controller output 
    return pid->out;

};