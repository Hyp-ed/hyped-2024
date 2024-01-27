"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Behaviour = void 0;
var helpers_1 = require("./utils/helpers");
/*
Planned File Structure

Each of the main response variables obeys different physical laws, and functions can be created to
predict and govern their change over time, after the pod starts to speed up from rest. Constraints come
in the form of the allowable numerical ranges for each variable.

Some variables can be analytically calculated, such as acceleration from basic calculus given a set of
initial conditions and desired steady-state cruising speed. Temperature is more arbitrary, it goes up with
an increase of kinetic energy but to what extent is up to the programmer.
*/
/**
* this class instantiates different data generation methods for a certain sensor
*/
var Behaviour = /** @class */ (function () {
    function Behaviour() {
    }
    /**
     * Method to track and generate values for displacement, velocity, acceleration
     * - Acceleration cannot exceed 5m/s^2 or drop below 0 during a run
     * - Velocity is controlled by pod operator and has limits of 0 and 50m/s
     * The pod will follow a logarithmic curve to reach near-maximum velocity
     * Once at desired speed, acceleration will drop to 0 and pod maintains near-constant velocity
     *   with some noise
     * This noise can bring acceleration under its critical minimum, so a moving average is employed
     *   so as to ignore noise
     * @param data all sensor data
     * @returns array of [displacement, velocity, acceleration]
     */
    Behaviour.motionSensors = function (data) {
        console.log('update time:', this.dt);
        // let prevDisp = data.displacement.currentValue;
        var prevVel = data.velocity.currentValue;
        // let prevDiso = data.velocity.currentValue;
        console.log('dt & timestep');
        console.log(this.dt);
        console.log(this.timestep);
        // let prevAccl = data.acceleration.currentValue;
        // Logistic function params
        var t = this.timestep;
        var maxVel = data.velocity.limits.critical.high;
        var startVel = 0.1 * maxVel;
        var growthRate = 0.444; // these params ensure accl. doesn't exceed 5
        var timeOfInflection = 15;
        var maxAccl = data.acceleration.limits.critical.high;
        // Logistic equation and kinematics
        var vel = (maxVel - startVel) / (1 + Math.exp(-growthRate * (t - timeOfInflection))) + startVel;
        var accl = (vel - prevVel) / this.dt >= maxAccl ? maxAccl
            : (vel - prevVel) / this.dt;
        var disp = data.displacement.currentValue + (vel * this.dt
            + 0.5 * accl * Math.pow(this.dt, 2));
        return [disp, vel, accl];
    };
    /**
     * Follows a rough sine wave with frequency increasing with pod velocity
     * As the timestep of 500ms is much higher than the time period of switching polarity,
     *  the resulting data will not actually resemble a sine wave as it's recording data once
     *  every few hundred/thousand polarity switches
     * @param data
     * @returns calculated value
     */
    Behaviour.hallEffect = function (data) {
        // const velocity = .getData();
        // Sine wave of frequency = fn(velocity) (directly proportional)
    };
    /**
     * Begins at 0 at rest
     * Remains at 0 until certain velocity threshold is reached
     * After that, begins to levitate until setpoint is achieved
     * Then remains at that level with small noise fluctuations
     * @param data
     * @returns calculated value
     */
    Behaviour.levitationHeight = function (data) {
        if (data.velocity.currentValue < this.levSpeedThreshold) {
            return 0;
        }
        var levSetpoint = (0, helpers_1.averageLimits)(data, data.levitation_height.key);
        console.log("Lev. Setpoint:", levSetpoint);
    };
    Behaviour.keyence = function (data) {
    };
    // generates completely random values within range limits
    Behaviour.generateRandomValues = function (data) {
        for (var sensor in data) {
            var prevVal = data[sensor].currentValue;
            var range = Math.abs(Object.values(data[sensor].limits.critical)
                .reduce(function (acc, c) { return acc - c; }));
            data[sensor].currentValue = data[sensor].format == 'float'
                ? parseFloat((Math.random() * range + data[sensor].limits.critical.low).toFixed(2))
                : Math.floor(Math.random() * (range + 1)) + data[sensor].limits.critical.low;
        }
        // return data;
    };
    Behaviour.levSpeedThreshold = 10; // arbitrary velocity value at which pod begins to levitate
    return Behaviour;
}());
exports.Behaviour = Behaviour;
