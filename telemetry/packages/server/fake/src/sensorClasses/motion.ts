import { Sensor } from '../baseSensor';
import { LiveReading, Readings, utils, measurements } from '../index';

export class Motion extends Sensor {
  protected displacement: number;
  protected velocity: number;
  protected acceleration: number;
  private velocityStState = 0.95;

  /**
   * Constructor for Motion class
   * @param accelerometer motion-type sensor data in LiveReading format (from sensorData object)
   * @param velocityStState Desired steady state velocity as percentage of maximum allowable velocity
   */
  constructor(accelerometer: LiveReading) {
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
    const velocityEstimate = utils.logistic(
      t,
      measurements.velocity.limits.critical.high * this.velocityStState, // Setpoint for velocity, reduced by 5% to allow for added noise
      0.4, // Exponential growth rate factor
      12.5, // Time of inflection that ensures acceleration peaks at its limiting operating value
    );
    console.log('vel estimate:', velocityEstimate);
    console.log(
      `|\taccl: ${utils.round2DP(
        this.acceleration,
      )} m/s² \tvel: ${utils.round2DP(
        this.velocity,
      )} m/s \tdisp: ${utils.round2DP(this.displacement)} m\t\n`,
    );

    // Calculate acceleration as the rate of change of logistic-fitted velocity
    let accelerometerReading =
      (velocityEstimate - this.velocity) / this.delta_t;
    // Assert reading is not above critical limit
    accelerometerReading =
      accelerometerReading >= this.limits.critical.high
        ? this.limits.critical.high
        : accelerometerReading;
    accelerometerReading += utils.gaussianRandom(this.rms_noise);

    // Set instance vars to current values using dv*dt and dx*dt
    // Use trapezoidal rule to estimate velocity and displacement
    const avgAcceleration = (accelerometerReading + this.acceleration) / 2;
    this.velocity += avgAcceleration * this.delta_t;
    this.displacement += this.velocity * this.delta_t;
    // Finally, update the acceleration value
    this.acceleration = accelerometerReading;
    // Return the three variables of interest
    return {
      acceleration: this.acceleration,
      velocity: this.velocity,
      displacement: this.displacement,
    };
  }
}
