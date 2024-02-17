import { Sensor } from '../baseSensor';
import { LiveReading, Readings, utils, measurements } from '../../index';

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
    // console.log(`Running getData at ${t} seconds`);
    const velocityEstimate = utils.logistic(
      t,
      measurements.velocity.limits.critical.high * this.velocityStState, // setpoint for velocity, reduced by 5% to allow for added noise
      0.4, // exponential growth rate factor
      12.5, // time of inflection that ensures acceleration peaks at its limiting operating value
    );
    console.log('args for logistic:', [...arguments]);
    console.log(t);
    console.log(measurements.velocity.limits.critical.high);
    console.log('vel estimate:', velocityEstimate);

    // calculate acceleration as the rate of change of logistic-fitted velocity
    let accelerometerReading =
      (velocityEstimate - this.velocity) / this.sampling_time;
    // assert reading is not above critical limit
    console.log('\naccelerometerReading: ', accelerometerReading)
    ;
    accelerometerReading =
      accelerometerReading >= this.limits.critical.high
        ? this.limits.critical.high
        : accelerometerReading;
    accelerometerReading += utils.gaussianRandom(this.rms_noise);

    console.log('\naccelerometerReading: ${accelerometerReading}');
    
    // Set instance vars to current values using dv*dt and dx*dt
    // Use trapezoidal rule to estimate velocity and displacement
    const avgAcceleration = (accelerometerReading + this.acceleration) / 2;
    this.velocity += avgAcceleration * this.sampling_time;
    this.displacement += this.velocity * this.sampling_time
    // Finally, update the acceleration value
    this.acceleration = accelerometerReading;
    
    for (const msmt in [this.velocity, this.acceleration, this.displacement]) {
      
    };

    console.log('\naccelerometerReading\t next iteration coming up ...\n: ');
    // Return the three variables of interest
    return {
      acceleration: this.acceleration,
      velocity: this.velocity,
      displacement: this.displacement,
    };
  }
}
