import { Sensor, Readings, SensorInstance, sensorData, sensors } from '..';
import { Utilities } from './sensorUtils';

import mqtt from 'mqtt';



export class SensorManager {
  // Create an array to store sensor instances
  private sensors: SensorInstance<
    (typeof sensors.default)[keyof typeof sensors.default]
  >[] = [];

  // Record the sampling intervals for each sensor
  private samplingTimes: Record<string, number> = {};

  // Global clock for simulation runtime
  private globalTime = 0;

  // Mqtt client
  private client: mqtt.MqttClient;

  /**
   * Single instantiation of this class for each run
   * @param sensorsToRun user-defined array of sensor names to be run in the current simulation. The exclusion
   * of sensors from whose values others are derived does not preclude their data generation, but solely their
   * publication to the server. However, their values will only be calculated as needed, not in accordance with
   * their sampling frequencies. The items in the array reflect the keys of sensorData.
   */
  constructor(private sensorsToRun: string[]) {
    // Create sensor instances
    this.instantiateSensors();
    // Store the sampling times for the current run's sensor type in accessible object
    // const samplingTimes: Record<string, number> = {};
    this.sensorsToRun.forEach((name: string) => {
      this.samplingTimes[name] = sensorData[name].sampling_time;
    });

    this.client = mqtt.connect('mqtt://localhost:1883');
  }

  /**
   * Runs and manages the entire time series of data generation
   * Updates global variables each iteration
   * Kills program once runTime has been reached
   * @param runTime
   * @param random boolean input set by user, determines process of data generation (random or the default of mathematically realistic)
   */
  public generateData(runTime: number, random = false): void {
    // Calculate the base interval (in ms) using the lowest common multiple of the various sampling times
    const interval = Utilities.gcd(Object.values(this.samplingTimes));
    // Run setInterval to generate and upload the data
    // This essentially acts as a while loop, but using real time (eliminating the chance of inifintie loops)
    const simulationInterval = setInterval(() => {
      // Preset all 'sampled' flags to false
      this.resetSampledState();

      this.sensors.forEach((sensor) => {
        // Generate data if this timestep corresponds to each sensor's sampling time
        if (this.globalTime % sensor.sampling_time == 0) {
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
          Object.entries(readings).forEach( ([measurement, value]) => {
            this.publishData(measurement, value);
          });
        }
        // At each timestep, each sensor should have a value corresponding to the global time
        // So those which haven't been sampled will remain at their last known value
      });

      // Increment time by the predefined interval
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
        // An array data type is used to preserve order of sensor instances 
        new sensors.default[sensorType](sensorData[sensorType]),
      );
    }
  }

  /**
   * At each timestep, reset all sensors' isSampled flags to back to the false state
   */
  private resetSampledState(): void {
    Object.keys(Sensor.isSampled).forEach( sensor => Sensor.isSampled[sensor] = false );
  }

  /**
   * Uploads data through MQTT broker to the frontend
   * The properties of the sensors' readings objects are the keys which are appended to the topic path, i.e. ...measurements/[key]
   * So simply append the key and publish the value as the payload
   * Subscribed clients extract values using payload[measurementKey]
   */
  private publishData(measurement: string, reading: number): void {
    this.client.publish(
      `hyped/pod_1/measurements/${measurement}`,
      reading,
      { qos: 1 },
      (err: any) => {
        if (err) {
          console.log(`MQTT publish error: [LOG] (pod_1) ${err}`);
        }
      },
    );
  }
  
}