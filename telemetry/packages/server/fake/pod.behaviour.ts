import { SensorData } from './data-gen';
import { DataManager } from './utils/data-manager';

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
export class Behaviour {
    // readonly sensor: RangeMeasurement
    public static timestep: number // current iteration
    private static levSpeedThreshold: number = 10 // arbitrary velocity value at which pod begins to levitate
        
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
    public static motionSensors(data: SensorData, dt: number): [number, number, number] {
        let prevDisp = data.displacement.currentVal;
        let prevVel = data.velocity.currentVal;
        let prevAccl = data.acceleration.currentVal;
        
        // Logistic function params
        const t = this.timestep;
        const maxVel = data.velocity.limits.critical.high;
        const growthRate = 0.4; // these params ensure accl. doesn't exceed 5
        const timeOfInflection = 12.5;
        
        const maxAccl = data.acceleration.limits.critical.high;
        
        const vel = maxVel / (1 + Math.exp(-growthRate * (t - timeOfInflection)))
        const accl = (vel - prevVel) / dt >= maxAccl ?
            maxAccl : (vel - prevVel) / dt;
        const disp = vel * t + 0.5 * accl * t ** 2;

        return [disp, vel, accl]
    }

    /**
     * Follows a rough sine wave with frequency increasing with pod velocity
     * As the timestep of 500ms is much higher than the time period of switching polarity,
     *  the resulting data will not actually resemble a sine wave as it's recording data once 
     *  every few hundred/thousand polarity switches
     * @param data
     * @returns calculated value
     */
    static hallEffect(data: SensorData) {
        // const velocity = .getData();
        // Sine wave of frequency = fn(velocity) (directly proportional)
    }

    /**
     * Begins at 0 at rest
     * Remains at 0 until certain velocity threshold is reached
     * After that, begins to levitate until setpoint is achieved
     * Then remains at that level with small noise fluctuations
     * @param data
     * @returns calculated value
     */
    static levitationHeight(data: SensorData) {

    }

    static keyence(data: SensorData) {

    }

    // generates completely random values within range limits
    static generateRandomValues(data: SensorData): void {
        for (const sensor in data) {
            const prevVal = data[sensor].currentVal;
            const range = Math.abs(
                Object.values(data[sensor].limits.critical)
                    .reduce( (acc, c) => acc - c)
            )
            data[sensor].currentVal = data[sensor].format == 'float'
                ? parseFloat((Math.random() * range + data[sensor].limits.critical.low).toFixed(2))
                : Math.floor(Math.random() * (range + 1)) + data[sensor].limits.critical.low
        }
    }
}