import {
  LiveReading,
  Readings,
  SensorInstance,
  sensorData,
  sensors,
  Sensor,
} from '..';
import { Utilities } from './sensorUtils';

export class SensorMap {
  // Create an array to store sensor instances
  private sensors: SensorInstance<
    (typeof sensors.default)[keyof typeof sensors.default]
  >[] = [];
  // Record the sampling intervals for each sensor
  private samplingTimes: Record<string, number> = {};

  // Define dynamic variables, updated each timestep
  // This object refers to sensors' sampling times to monitor the next time for each sensors' reading (in real time)
  private latestReadings: Record<string, Readings> = {};
  // Boolean values to indicates whether each sensor has been sampled at current timestep
  private isSampled: Record<string, boolean> = {};
  // Global clock for simulation runtime
  private globalTime: number = 0;

  /**
   * Single instantiation of this class for each run
   * @param runSensors user-defined array of sensor names to be run in the current simulation. The exclusion
   * of sensors from whose values others are derived does not preclude their data generation, but solely their
   * publication to the server. However, their values will only be calculated as needed, not in accordance with
   * their sampling frequencies.
   */
  // constructor(private runSensors: string[]) { // use when you implement specific option logic
  constructor() {
    this.instantiateSensors();
    // console.log("ordered sensors: ", this.sensors);
    // console.log(sensors.default);
    // const a = this.sensorsOrdered[0] as typeof sensors.default.motion;

    // Store the sampling times for each sensor type in accessible object
    const samplingTimes: Record<string, number> = {};
    Object.entries(sensorData).forEach(
      ([name, sensor]: [string, LiveReading]) => {
        samplingTimes[name] = sensor.sampling_time;
      },
    );
  }

  /**
   * Run and manage the entire time series of data generation
   * Update global variables each iteration
   * Exit once runTime has been reached
   * @param runTime
   */
  public generateData(runTime: number, random: boolean = false): void {
    // Calculate the base interval (in ms) using the lowest common multiple of the various sampling times
    //
    const interval = Utilities.gcd(Object.values(this.samplingTimes));
    // Run setInterval to generate and upload the data
    // This essentially acts as a while loop, but using real time (which eliminates chance of inifintie loops!)
    const simulationInterval = setInterval(() => {
      this.resetSampledState();

      this.sensors.forEach((sensor) => {
        if (this.globalTime % sensor.sampling_time == 0) {
          // Get the sensors' output readings, passing in the time converted into seconds
          const readings: Readings = !random
            ? sensor.getData(this.globalTime / 1000)
            : sensor.getRandomData(this.latestReadings[sensor.type]);
          this.latestReadings[sensor.type] = readings;
          this.isSampled[sensor.type] = true;

          // MQTT.publish(data);
        }
      });

      // Advance the global time by the predefined interval
      this.globalTime += interval;
      if (this.globalTime >= runTime) {
        clearInterval(simulationInterval);
      }
    }, interval);
  }

  // Instantiate sensors with their respective data and store instances in array
  private instantiateSensors(): void {
    for (const sensorType in sensorData) {
      this.sensors.push(
        new sensors.default[sensorType](sensorData[sensorType]),
      );
      // console.log("sensor type:", sensorType);
    }
    // console.log('sensors:', this.sensors);
  }

  /**
   * At each timestep, reset all sensors' _activated flags to back to the false state
   */
  private resetSampledState(): void {
    Object.keys(this.isSampled).forEach(
      (sensor) => (this.isSampled[sensor] = false),
    );
  }

  /**
   * Observe the sensor data generation behaviour to determine the dependency tree
   * For now, this method will not be used as the sensors are chosen for the year and fixed,
   *   so 'hardcoding' is permissible, in the form of ordering the sensors appropriately in
   *   the array of sensor instances. But if this program were to become more generalised in
   *   the future, this function could be used to calculate the sensor 'hierarchy' in terms
   *   of fake data generation
   * Potential logic it could use is highlighed as pseudocode below
   */
  private getDependecies(sensorType: string): Record<string, any> {
    /*
        Loop through sensorData, assuming its an arbitrary 'dictionary' of sensors, yet unknown to the program
        Insert logic to test their functionality and monitor which other sensors are influenced by their getData
          methods. Those sensors, in the order in which their own methods were called, are the parent classes of
          the dependent sensor.
        E.g. To simulate the variety of pressure gauges, the method must be aware of the current motion (velocity),
          and temperature (thermodynamics and ideal gas laws). So its dependencies would be the classes Motion and
          Temperature. These would be stored in an array as a new sensor property called 'dependencies' or similar.
        Then when the pressure gauges are called upon to cough up some data for the telemetry team, the code would
          know exactly which sensor readings would need to be calculated to provide an accurate measurementy for
          pressure.
        */
    return {
      /* dependencies categorised by sensor */
    };
  }

  // Generate data for each sensor type
  generate(): void {
    // Record previous time; if at initial step then set to 0
    const prevTime = this.globalTime | 0;

    // Ensure the time is set to the next sampling time before the data generation loop
    // While this is an assignment operator, not an incremental, infinite loops are impossible
    //   because the while loop in main.ts runs for a finite, user-specified amount of seconds in
    //   real time.
    // Besides error handling, the logic to this code is to ensure that sensor readings are taken
    //   at every single timestep at which at least one sensor is due to generate data.
    // Depending on the variability of the sampling times, the overall frequency at which data is
    //   could vary significantly, hence this type of time increment logic.
    // this.time = Math.min(...Object.values(Sensor.nextSamplingTimes));

    // Set isSampled to false for all sensors before running loop
    Sensor.isSampled = Object.fromEntries(
      Object.keys(Sensor.isSampled).map((key) => [key, false]),
    );

    const delay = this.globalTime - prevTime;

    // Reset activated state for all sensors
    this.sensors.forEach((sensor, i) => {
      sensor.activated = false;
    });

    setTimeout(() => {
      this.sensors.forEach((sensor, i) => {
        if (this.globalTime < Sensor.nextSamplingTimes[sensor.type]) {
          return;
        }

        this.sensors[i].getData(this.globalTime);
        // this method also runs the update method

        // const sensorName = sensor.name.toLowerCase();
        // update readings for each sensor
        // MQTT.upload(this.sensorData[sensor].readings);

        // update dynamic sensor data

        // ALREADYB HAPPENING FOR INDIVIDUAL SENSORS IN SENSOR CLASS
        // Sensor.nextSamplingTimes[sensor.type] += Sensor.samplingTimes[sensor.type];
        // Sensor.isSampled[sensor.type] = true;
      });

      // Object.keys(this.sensorData).
    }, delay);

    // upload all data for the current timestep
  }

  generateRandom(): void {
    const prevTime = this.globalTime | 0;

    // Ensure the time is set to the next sampling time before the data generation loop
    this.globalTime = Math.min(...Object.values(Sensor.nextSamplingTimes));

    const delay = this.globalTime - prevTime;

    setTimeout(() => {
      // Loop through sensor types and update their readings data
      for (const sensor in this.sensorData) {
        // Skip sensors that haven't reached their next sampling time
        if (this.globalTime < Sensor.nextSamplingTimes[sensor]) {
          continue;
        }

        // Get new randomised values for each sensor
        for (const unit in this.sensorData[sensor]) {
          const { limits, rms_noise, format } = this.sensorData[sensor];
          this.sensorData[sensor].readings[unit] = Utilities.getRandomValue(
            limits,
            rms_noise,
            format,
          );
        }

        Sensor.isSampled[sensor] = true;

        // The motion sensors are a special case, as they consist of not just sensors but also derived measurements
        // Only the derived measurements need to be uploaded, and while the accelerometers will be randomised, the
        //  motion variables will be calculated from them
        if (sensor == 'motion') {
          const {
            accelerometer_1,
            accelerometer_2,
            accelerometer_3,
            accelerometer_4,
            ...data
          } = this.sensorData['motion'].readings;
          // MQTT.upload(data);
        } else {
          // MQTT.upload(this.sensorData[sensor].readings);
        }

        // Set the next sampling time for the sensor
        Sensor.nextSamplingTimes[sensor] += Sensor.samplingTimes[sensor];
      }
    }, delay);
  }
}

// private set time(nextTime) {
//     // this method should only be called once the nextSamplingTimes object has been updated
//     if (nextTime == this.globalTime) { return; }
//     this.globalTime = nextTime;
// }

// get time() {
//     return this.globalTime;
// }

// const run_1 = new SensorMap();

// Notes or comments brought down here to make code cleaner - summarise anything important and add to README file.

// Time-variant objects
// Records the actual time each sensor should be sampled next
// nextSamplingTimes: typeof this.samplingTimes;
// Records whether each sensor has been sampled at the current time
// isSampled: Record<string, boolean>;

// Dynamic object which store the next sampling times for each sensor, initialised to the first timestep
// Every iteration, it updates by incrementing each sensor's value by its fixed sampling time
// Sensor.nextSamplingTimes = {...samplingTimes};
// Sensor.isSampled = Object.fromEntries(Object.keys(samplingTimes).map( (key) => [key, false]));

// this.sensorClasses = [Motion, Temperature, Pressure, Magnetism, Keyence, Resistance, Levitation];

// instantiated sensor classes must be stored in an array to maintain their order
// I.e. Motion -> Temperature and Magnetism -> rest
// this.sensors = this.sensorClasses.map( SensorCls => new SensorCls(
//     sensorData[SensorCls.name.toLowerCase()]
// ) );

// There is a specific order in which the data must be generated
// e.g. motion and temperature data must be generated before pressure data
// However, this issue is evaded as any required data not generated yet will be, and that reading will
//   be recorded for that sensor should it also be due to give a reading at that time
