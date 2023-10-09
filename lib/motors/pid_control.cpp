#include <iostream>

#include <iostream>

class PIDController {
public:
    PIDController(double Kp, double Ki, double Kd){
    Kp_ = Kp;
    Ki_ = Ki;
    Kd_ = Kd;
    previous_error_ = 0;
    integral_ = 0;
}

    double ComputeControlSignal(double setpoint, double current_frequency) {
        double error = setpoint - current_frequency;
        integral_ += error;
        double derivative = error - previous_error_;

        // PID control equation
        double control_signal = Kp_ * error + Ki_ * integral_ + Kd_ * derivative;

        // Store error for the next iteration
        previous_error_ = error;

        return control_signal;
    }

private:
    double Kp_;  
    double Ki_;  
    double Kd_;  

    double previous_error_;  
    double integral_;
};

int main() {
    double Kp = 0.1;
    double Ki = 0.01;
    double Kd = 0.01;

    double setpoint_frequency = 60.0;

    double current_frequency = 55.0;

    PIDController pid_controller(Kp, Ki, Kd);

    double control_signal = pid_controller.ComputeControlSignal(setpoint_frequency, current_frequency);

    std::cout << "Control Signal: " << control_signal << std::endl;

    return 0;
}
