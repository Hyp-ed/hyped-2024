#include "PID.hpp"


void PIDController::PIDController_Init() {
    	
    /* Clear controller variables */
	integrator = 0.0f;
	prevError  = 0.0f;
	differentiator  = 0.0f;
	prevMeasurement = 0.0f;
    
    out = 0.0f;

};

float PIDController::PIDController_Update(float setpoint, float measurement) {

	// Error signal 
    float error = setpoint - measurement;

	// Proportional term
    float proportional = Kp * error;

	// Integral term
	integrator = integrator + 0.5f * Ki * T * (error + prevError);

	// Integrator Anti wind-up (dynamic integrator clamping)
    if (integrator > limMaxInt) {

        integrator = limMaxInt;

    } else if (integrator < limMinInt) {

        integrator = limMinInt;

    }


	// Derivative Term (optional) with low pass filter		
    differentiator = -(2.0f * Kd * (measurement - prevMeasurement)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * tau - T) * differentiator)
                        / (2.0f * tau + T);



	// Set controller output and limit outputs if needed
    out = proportional + integrator + differentiator;

    if (out > limMax) {

        out = limMax;

    } else if (out < limMin) {

        out = limMin;

    }

	// Store error and measurement for later use 
    prevError       = error;
    prevMeasurement = measurement;

	// Return controller output 
    return out;

};