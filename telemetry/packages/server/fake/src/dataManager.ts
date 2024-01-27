import {
  Sensor,
  Readings,
  SensorInstance,
  sensorData,
  sensors,
  utils,
  trackLength,
} from '.';

import MQTT from 'mqtt/dist/mqtt';
export class SensorManager {
  // Create an array to store sensor instances
  private sensors: SensorInstance<(typeof sensors)[keyof typeof sensors]>[] =
    [];
  // Record the sampling intervals for each sensor
  private samplingTimes: Record<string, number> = {};
  // Global clock for simulation runtime
  private globalTime = 0;
  // Mqtt client
  private client: MQTT.MqttClient;

  // Sensor dependencies (sub-classes)
  private dependencies: Record<string, string | null> = {
    motion: null,
    keyence: 'motion',
    temperature: 'motion', // possibly pressure too, avoid circular logic though
    pressure: 'temperature',
    resistance: 'temperature',
    magnetism: 'motion',
    levitation: 'magnetism',
  };

  /**
   * Sensor manager singleton class, controls and connects all individual sensor classes
   * @param sensorsToRun user-defined array of sensor names to be run in the current simulation
   */
  constructor(private sensorsToRun: string[]) {
    // Create sensor instances
    this.instantiateSensors(this.sensorsToRun);
    // Store the sampling times for the current run's sensor type in accessible object
    this.sensorsToRun.forEach((s: string) => {
      this.samplingTimes[s] = sensorData[s].sampling_time;
    });

    this.client = MQTT.connect('MQTT://localhost:1883');
  }

  /**
   * Runs transient data generation
   * Updates global variables each iteration
   * End program once runTime has been reached
   * @param random boolean input set by user, allows for completely random data
   */
  public generateData(random = false): void {
    // Calculate base sampling interval using lowest common divisor of all sensors' sampling periods
    const interval = utils.gcd(Object.values(this.samplingTimes));

    const simulationInterval = setInterval(() => {
      // Reset all 'sampled' flags to false
      this.resetSampledState();

      console.log(`\n***** \t *****\nTime: ${this.globalTime * 0.001} s`);

      this.sensors.forEach((sensor) => {
        // Generate data if current time corresponds to sensor's sampling time
        if ((this.globalTime / 1000) % sensor.delta_t == 0) {
          // Get the sensors' output readings
          const readings: Readings = !random
            ? sensor.getData(this.globalTime / 1000) // convert time to seconds for calculations
            : sensor.getRandomData(Sensor.lastReadings[sensor.type]);
          // Store latest readings and set isSampled flag to true
          Sensor.lastReadings[sensor.type] = readings;
          Sensor.isSampled[sensor.type] = true;

          // Publish each of the current sensor type's reading values under the topic of
          //   its corresponding measurement key to the MQTT broker
          Object.entries(readings).forEach(([measurement, value]) => {
            this.publishData(measurement, value.toString());
          });
          // Log output
          if (this.sensorsToRun.includes(sensor.type)) {
            console.log(readings);
          }
        }
      });

      // Implement exit condition
      if (Sensor.lastReadings.motion.displacement >= trackLength) {
        clearInterval(simulationInterval);
        console.log('\n\n*** Simulation complete ***\n');
        console.log('Final state:', Sensor.lastReadings, '\n');
        process.exit(0);
      }

      this.globalTime += interval;
    }, interval);
  }

  /**
   * Instantiate sensors and their required superclasses and store instances in array
   */
  private instantiateSensors(sensorsToRun: string[]): void {
    // Record sensors names to be added to instances array
    const activeSensors: Set<string> = new Set();
    // Function to activate all sensors required
    const getActiveSensors = (name: string): void => {
      if (this.dependencies[name] == null) {
        activeSensors.add(name);
      } else {
        getActiveSensors(this.dependencies[name] as string);
        activeSensors.add(name);
      }
      return;
    };
    // Populate set
    sensorsToRun.forEach((s) => getActiveSensors(s));
    // Define sensor instances
    // Correct sorting is automatic as recursion forces all parent class sensors to be
    //   added before their inheriting classes
    activeSensors.forEach((s) =>
      this.sensors.push(new sensors[s](sensorData[s])),
    );
  }

  /**
   * Reset all sensors' isSampled flags to false on each iteration
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
  private publishData(measurement: string, reading: string): void {
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
