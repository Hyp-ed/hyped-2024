import { Sensor } from "../baseSensor";
import { LiveReading, Readings, Utilities, measurements } from "../../index";

export class Motion extends Sensor {

    protected velocity: number;
    protected acceleration: number;
  
    constructor(accelerometer: LiveReading) {
      super(accelerometer);
      // record the velocity and acceleration for ease of access by dependent subclasses
      this.acceleration = accelerometer.readings.acceleration;
      this.velocity = accelerometer.readings.velocity;
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
        measurements.velocity.limits.critical.high * 0.95, // setpoint for velocity, reduced by 5% to allow for added noise
        0.4, // exponential growth rate factor
        12.5, // time of inflection that ensures acceleration peaks at its limiting operating value
      );
    
      // calculate acceleration as the rate of change of logistic-fitted velocity
      let accelerometerReading =
        (velocityEstimate - Sensor.lastReadings.motion.velocity) /
        this.sampling_time;
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
        velocity: (Sensor.lastReadings.motion.velocity +=
          accelerometerReading * this.sampling_time),
        displacement: (Sensor.lastReadings.motion.displacement +=
          Sensor.lastReadings.motion.velocity * this.sampling_time),
      };
    }
  
}