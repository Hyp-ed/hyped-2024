import { LiveReading, Readings, Limits, Utilities } from '../index';

export abstract class Sensor {
  // Define static objects, updated each timestep //
  // Records the actual time each sensor should be sampled next
  // This object refers to sensors' sampling times to monitor the next time for each sensors' reading (in real time)
  public static nextSamplingTimes: Record<string, number>;
  // Records whether each sensor has been sampled at the current time with a boolean flag for each
  public static isSampled: Record<string, boolean>;
  // Stores most recent sensor readings for all sensors, accessible by all sensors
  // Null is used to indicate that the sensor has not been sampled at the current time
  public static lastReadings: Record<string, Readings> = {}; 

  // Sensor properties
  readonly type: string; // sensor type (same as the name of the object in sensorData)
  // readonly key: string; // sensor id, but this not as a top-level property, instead the readings all have unique keys
  readonly format: 'float' | 'integer'; // for random ternary logic (keyence is integer, rest are float)
  // readonly unit: string; // if we want to display units on UI, probably not necessary as front end will already do this
  readonly limits: Limits;
  readonly rms_noise: number;
  readonly sampling_time: number;
  readonly quantity: number;
  // readonly quantity: number; // uncomment if needed somewhere like averaging

  // Variable sensor data
  protected time: number; // current time in seconds
  protected _activated: boolean = false; // flag to indicate whether the sensor has been sampled at the current time

  // Extract relevant properties from sensor data entries
  constructor({
    type,
    format,
    limits,
    rms_noise,
    sampling_time,
    readings,
    quantity
  }: LiveReading) {
    Object.assign(this, { type, format, limits, rms_noise, sampling_time, quantity });
    this.time = 0;
    console.log('readings:', readings);
    // Add initial sensor values to global readings object
    Sensor.lastReadings[this.type] = readings;
  }

  /**
   * Main data gen method shared by all sensors
   * Returns the Readings object, filtered into the values to be published with MQTT
   * For motion, only acceleration, velocitity and displacement are uploaded
   * Accelerometers are used to estimate readings, then these values are
   *   propagated to the three variables above
   * @param t 
   */
  abstract getData(t: number): Readings;
  // { ... this.update(newReadings) , return; }

  /*
    
    Each instance, will run getData() generate values for their respective measurements
    Then, this.update will run after generating a value in order to publish data to MQTT

    */

  // Update time and set isSampled flag to true for reference by interested sensor classes
  protected update(newReadings: Readings): void {
    // Set flag to true to indicate that the sensor has been sampled at the current time
    this.activated = true;
    // Update readings with new values
    Sensor.lastReadings[this.type] = newReadings;

    // Only upload to server, and update time and state properties if the sensor was updated
    //   directly, i.e. not as a prerequisite for another sensor's data generatinon calculations
    // Other sensors may depend on the motion sensors, but if the motion sensors
    //   have not reached their next sampling time, this calculation should not be recorded

    // isSampled and readings are set to true by default so other dependent sensors can access
    //   the new values without having to update the parent sensor method again needlessly
    if (this.sampling_time % this.sampling_time == 0) {
      this.time += this.sampling_time;

      // update global sensor state properties
      Sensor.nextSamplingTimes[this.type] += this.sampling_time;
      Sensor.isSampled[this.type] = this.activated;

      // publish to server
      // MQTT.publish(this.readings);
    }
  }

  protected getRandomData(prevValue: number, readings: Readings): Readings {
    for (const unit in readings) {
      readings[unit] = Utilities.getRandomValue(
        prevValue,
        this.limits,
        this.rms_noise,
        this.format,
      );
    }
    return readings;
  }

  // Not needed, performed by sensorManager loop in global isSampled
  // set activated(newState: boolean) {
  //   this._activated = newState;
  // }

  protected sensorisSampled(sensor: string) {
    return Sensor.isSampled[sensor]
  }
}