import {
  Sensor,
  Readings,
  SensorInstance,
  sensorData,
  sensors,
  utils,
  trackLength,
} from '.';

// import MQTT from 'mqtt'; // error npm ERR! command sh -c husky install; husky: command not found
export class SensorManager {
  // Create an array to store sensor instances
  private sensors: SensorInstance<(typeof sensors)[keyof typeof sensors]>[] =
    [];

  // Record the sampling intervals for each sensor
  private samplingTimes: Record<string, number> = {};

  // Global clock for simulation runtime
  private globalTime = 0;

  // Mqtt client
  // private client: MQTT.MqttClient;

  // Sensor dependencies (sub-classes)
  private sensorDependencies: Record<string, string[]> = {
    motion: [],
    keyence: ['motion'],
    temperature: ['motion'], // possibly pressure too, avoid circular logic though
    pressure: ['motion', 'temperature'],
    resistance: ['motion', 'temperature'],
    magnetism: ['motion'],
    levitation: ['motion', 'magnetism'],
  };

  /**
   * Single instantiation of this class for each run
   * @param sensorsToRun user-defined array of sensor names to be run in the current simulation. The exclusion
   * of sensors from whose values others are derived does not preclude their data generation, but solely their
   * publication to the server. However, their values will only be calculated as needed, not in accordance with
   * their sampling frequencies. The items in the array reflect the keys of sensorData.
   */
  constructor(private sensorsToRun: string[]) {
    // Create sensor instances
    this.instantiateSensors(this.sensorsToRun);
    // Store the sampling times for the current run's sensor type in accessible object
    // const samplingTimes: Record<string, number> = {};
    this.sensorsToRun.forEach((name: string) => {
      this.samplingTimes[name] = sensorData[name].sampling_time;
    });

    // this.client = MQTT.connect('MQTT://localhost:1883');
  }

  /**
   * Runs and manages the entire time series of data generation
   * Updates global variables each iteration
   * Kills program once runTime has been reached
   * @param runTime
   * @param random boolean input set by user, determines process of data generation (random or the default of mathematically realistic)
   */
  public generateData(random = false): void {
    // Calculate the base interval (in ms) using the lowest common multiple of the various sampling times
    const interval = utils.gcd(Object.values(this.samplingTimes));
    // Run setInterval to generate and upload the data
    // This essentially acts as a while loop, but using real time (eliminating the chance of inifintie loops)

    console.log(`\nthis.sensors: ${this.sensors}\n`);
    const simulationInterval = setInterval(() => {
      // Preset all 'sampled' flags to false
      this.resetSampledState();

      console.log(`\nTime: ${this.globalTime * 0.001} s`);

      this.sensors.forEach((sensor) => {
        // Generate data if this timestep corresponds to each sensor's sampling time
        if ((this.globalTime * 1000) % sensor.delta_t == 0) {
          // Get the sensors' output readings, passing in the time converted into seconds
          // If user set random is true, use the random data generation method
          const readings: Readings = !random
            ? sensor.getData(this.globalTime / 1000) // convert time into seconds for calculations
            : sensor.getRandomData(Sensor.lastReadings[sensor.type]);
          // Store latest readings and set isSampled flag to true
          Sensor.lastReadings[sensor.type] = readings;
          Sensor.isSampled[sensor.type] = true;

          // Publish each of the current sensor type's reading values under the topic of
          //   its corresponding measurement key to the MQTT broker
          // Object.entries(readings).forEach(([measurement, value]) => {
          //   this.publishData(measurement, value.toString());
          // });

          // this.logData(readings);
          console.log(readings);
        }

        // At each timestep, each sensor should have a value corresponding to the global time
        // So those which haven't been sampled will remain at their last known value
      });

      // Implement exit condition
      if (Sensor.lastReadings.motion.displacement >= trackLength) {
        clearInterval(simulationInterval);
        console.log('Simulation complete');
        process.exit(0);
      }

      this.globalTime += interval;
    }, interval);
  }

  private logData(data: Readings): void {
    for (const measurement in data) {
      console.log(`${measurement}: ${data[measurement]}`);
    }
    console.log(`\n`);
  }

  // Instantiate sensors with their respective data and store instances in array
  // Create instances of those selected by user and any parent-class sensors
  // An array data type is used to preserve order of sensor instances
  private instantiateSensors(sensorsToRun: string[]): void {
    // Record added sensors as strings in unique set
    const activeSensors: Set<string> = new Set();
    // Add dependent sensors first as their instance props are used in subclass constructors
    for (const sensorType of sensorsToRun) {
      if (!activeSensors.has(sensorType)) {
        this.sensors.push(
          ...this.sensorDependencies[sensorType]
            .filter((dpndSensor: string) => !activeSensors.has(dpndSensor))
            .map(
              (dpndSensor: string) =>
                new sensors[dpndSensor](sensorData[dpndSensor]),
            ),
          new sensors[sensorType](sensorData[sensorType]),
        );
        console.log('Sensor dependencies:\n');
        console.log([...this.sensorDependencies[sensorType]]);
        activeSensors.add(sensorType);
      }
    }
  }

  /**
   * At each timestep, reset all sensors' isSampled flags to back to the false state
   */
  private resetSampledState(): void {
    Object.keys(Sensor.isSampled).forEach(
      (sensor) => (Sensor.isSampled[sensor] = false),
    );
  }

  /**
   * Uploads data through MQTT broker to the frontend
   * The properties of the sensors' readings objects are the keys which are appended to the topic path, i.e. ...measurements/[key]
   * So simply append the key and publish the value as the payload
   * Subscribed clients extract values using payload[measurementKey]
   */
  // private publishData(measurement: string, reading: string): void {
  //   this.client.publish(
  //     `hyped/pod_1/measurements/${measurement}`,
  //     reading,
  //     { qos: 1 },
  //     (err: any) => {
  //       if (err) {
  //         console.log(`MQTT publish error: [LOG] (pod_1) ${err}`);
  //       }
  //     },
  //   );
  // }
}
