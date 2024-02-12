import { Readings, SensorInstance, sensorData, sensors } from '..';
import { Utilities } from './sensorUtils';

// import MQTTProvider from '../../../ui/app/context/mqtt'
import mqtt from 'mqtt';

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
    // optional until specific sensor choice logic is added
    this.instantiateSensors();
    // Store the sampling times for the current run's sensor type in accessible object
    const samplingTimes: Record<string, number> = {};
    this.sensorsToRun.forEach((name: string) => {
      samplingTimes[name] = sensorData[name].sampling_time;
    });

    this.client = mqtt.connect('mqtt://localhost:1883');
  }

  /**
   * Run and manage the entire time series of data generation
   * Update global variables each iteration
   * Kill program once runTime has been reached
   * @param runTime
   * @param random boolean input set by user, determines process of data generation (random or the default of mathematically realistic)
   */
  public generateData(runTime: number, random = false): void {
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
            ? sensor.getData(this.globalTime / 1000) // convert time into seconds for calculations
            : sensor.getRandomData(this.latestReadings[sensor.type]);
          this.latestReadings[sensor.type] = readings;
          this.isSampled[sensor.type] = true;

          this.publishData(sensor.type, readings);
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
    console.log('sensors:', this.sensors);
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
   * Uploads data through MQTT broker to the frontend
   * The properties of the sensors' readings objects are the keys which are appended to the topic path, i.e. ...measurements/[key]
   * So simply append the key and publish the value as the payload
   *
   * Subscribing topics must extract values using payload[measurementKey]
   */
  private publishData(measurement: string, readings: Readings): void {
    this.client.publish(
      `hyped/pod_1/measurements/${measurement}`,
      JSON.stringify(readings),
      (err) => {
        if (err) {
          console.log(`MQTT publish error: [LOG] (pod_1) ${err}`);
        }
      },
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

  /*
  private getDependecies(sensorType: string): Record<string, any> {
    
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
        
    return {
    dependencies categorised by sensor
    };

    */

}
