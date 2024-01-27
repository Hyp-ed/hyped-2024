import { SensorData } from '../../types/src';
import { DataManager } from './utils/data-manager';
import { averageLimits } from './utils/helpers';

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
  public static timestep: number; // current iteration
  public static dt: number; // iteration update time (or delta t)

  private static levSpeedThreshold: number = 10; // arbitrary velocity value at which pod begins to levitate

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
  public static motionSensors(data: SensorData) {
    console.log('update time:', this.dt);
    // let prevDisp = data.displacement.currentValue;
    let prevVel = data.velocity.currentValue;
    // let prevDiso = data.velocity.currentValue;
    console.log('dt & timestep');
    console.log(this.dt);
    console.log(this.timestep);
    // let prevAccl = data.acceleration.currentValue;

    // Logistic function params
    const t = this.timestep;
    const maxVel = data.velocity.limits.critical.high;
    const startVel = 0.1 * maxVel;
    const growthRate = 0.444; // these params ensure accl. doesn't exceed 5
    const timeOfInflection = 15;

    const maxAccl = data.acceleration.limits.critical.high;

    // Logistic equation and kinematics
    const vel =
      (maxVel - startVel) /
        (1 + Math.exp(-growthRate * (t - timeOfInflection))) +
      startVel;
    const accl =
      (vel - prevVel) / this.dt >= maxAccl
        ? maxAccl
        : (vel - prevVel) / this.dt;
    const disp =
      data.displacement.currentValue +
      (vel * this.dt + 0.5 * accl * this.dt ** 2);

    return [disp, vel, accl];
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
    if (data.velocity.currentValue < this.levSpeedThreshold) {
      return 0;
    }
    const levSetpoint = averageLimits(data, data.levitation_height.key);
    console.log('Lev. Setpoint:', levSetpoint);
  }

  static keyence(data: SensorData) {}

  // generates completely random values within range limits
  static generateRandomValues(data: SensorData): void {
    for (const sensor in data) {
      const prevVal = data[sensor].currentValue;
      const range = Math.abs(
        Object.values(data[sensor].limits.critical).reduce((acc, c) => acc - c),
      );
      data[sensor].currentValue =
        data[sensor].format == 'float'
          ? parseFloat(
              (
                Math.random() * range +
                data[sensor].limits.critical.low
              ).toFixed(2),
            )
          : Math.floor(Math.random() * (range + 1)) +
            data[sensor].limits.critical.low;
    }
    // return data;
  }
}
