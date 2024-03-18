#include <iostream>
#include <cmath>

#include "vector_controller.hpp"

namespace hyped :: motors {
    
    VectorController::VectorController() {
        Kp = 1.0;  
        Ki = 0.1; 
        IdRef = 0.0;  
        IqRef = 0.0;  
        theta = 0.0; 
    }

            // Measure stator currents and rotor velocity
            void VectorController::measureCurrentsAndVelocity(core::Float ia, core::Float ib, core::Float ic, core::Float velocity) {
                Ia = ia;
                Ib = ib;
                Ic = ic;
                rotorVelocity = velocity;
            }

            // Convert to 2-axis system
            void VectorController::convertTo2Axis() {
                // Perform transformation from 3-phase currents (Ia, Ib, Ic) to 2-axis (Id, Iq)
                // Implement the Clarke transformation
                core::Float Ialpha = Ia;
                core::Float Ibeta = (Ia + 2*Ib)/sqrt(3);
                
                // Rotate coordinate system to align with rotor flux
                // Calculate the transformation angle based on the rotor flux alignment
                // Implement the Park transformation
                Id = cos(theta) * Ialpha + sin(theta) * Ibeta;
                Iq = -cos(theta) * Ialpha + sin(theta) * Ibeta;    
            }

            // Inverse Park and Clarke
            void VectorController::inverse2Axis() {
                // Inverse Park
                core::Float Valpha = Vd * cos(theta) - Vq * sin(theta);
                core::Float Vbeta = Vd * sin(theta) + Vq * cos(theta);

                // Inverse Clarke
                // Will need to return something if we want to use at some point
                core::Float Va = Vbeta;
                core::Float Vb = (Vbeta + sqrt(3)*Valpha) / 2;
                core::Float Vc = (Vbeta + sqrt(3)*Valpha) / 2;
                
            }

            // PI control on variables with reference
            void VectorController::piControl() {
                core::Float IdError = IdRef - Id;
                core::Float IqError = IqRef - Iq;

                // rotor magnetizing flux
                core::Float integral_term_Id = Ki * IdError;
                Vd = Kp * IdError + integral_term_Id;

                // torque output of motor
                core::Float integral_term_Iq = Ki * IqError;
                Vq = Kp * IqError + integral_term_Iq;
            }

            void VectorController::inverseParkTransform() {
                core::Float Valpha = Vd * cos(theta) - Vq * sin(theta);
                core::Float Vbeta = Vd * sin(theta) + Vq * cos(theta);

                Vd = Valpha;
                Vq = Vbeta;
            }
        

        int main() {
            // Instantiate the motor controller
            VectorController motorCtrl;

            // Example: Set measured currents and rotor velocity
            motorCtrl.measureCurrentsAndVelocity(10.0, 5.0, 8.0, 150.0); 

            // Perform the vector control steps
            motorCtrl.convertTo2Axis();
            motorCtrl.piControl();
            motorCtrl.inverseParkTransform(); // after this, Vd and Vq would have the desired values
                                       // space vector modulation next?
            

            // Output or use the results as needed
            return 0;
            }
        }