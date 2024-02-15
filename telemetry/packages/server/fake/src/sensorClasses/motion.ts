import { Sensor } from '../baseSensor';
import { LiveReading, Readings, Utilities, measurements } from '../../index';

export class Motion extends Sensor {
  protected displacement: number;
  protected velocity: number;
  protected acceleration: number;

  /**
   * Constructor for Motion class
   * @param accelerometer motion-type sensor data in LiveReading format (from sensorData object)
   * @param velocityStState Desired steady state velocity as percentage of maximum allowable velocity
   */
  constructor(accelerometer: LiveReading, private velocityStState = 0.95) {
    super(accelerometer);
    // store the relevant deconstructed reading values into new variables for legibility
    const { displacement, velocity, acceleration } = Sensor.lastReadings.motion;
    Object.assign(this, { displacement, velocity, acceleration });
  }
  /**
   * Gets current reading from each accelerometer and calculates the other motion variables
   * Estimates velocity-time graph as a logistic function
   * @param t current time in seconds, used to calculate motion variables
   * @returns updated readings object containing acceleratio, velocity and displacement
   */
  getData(t: number): Readings {
    const velocityEstimate = Utilities.logistic(
      t,
      measurements.velocity.limits.critical.high * this.velocityStState, // setpoint for velocity, reduced by 5% to allow for added noise
      0.4, // exponential growth rate factor
      12.5, // time of inflection that ensures acceleration peaks at its limiting operating value
    );
    // calculate acceleration as the rate of change of logistic-fitted velocity
    let accelerometerReading =
      (velocityEstimate - this.velocity) / this.sampling_time;
    // assert reading is not above critical limit
    accelerometerReading =
      accelerometerReading >= this.limits.critical.high
        ? this.limits.critical.high
        : accelerometerReading;
    accelerometerReading += Utilities.gaussianRandom(this.rms_noise);
    // Return the three variables of interest,
    //   calculating velocity and displacement using dv*dt and dx*dt
    return {
      acceleration: accelerometerReading,
      velocity: (this.velocity += accelerometerReading * this.sampling_time),
      displacement: (this.displacement += this.velocity * this.sampling_time),
    };
  }
}
