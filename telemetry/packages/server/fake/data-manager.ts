import { RunData, ReadingsMap } from './src';
import { MQTT_BROKER_HOST } from '@/modules/core/config';
import mqtt from 'mqtt';

export class DataManager {
  private static instance: DataManager | null = null; // Static property to hold the single instance
  private _data: ReadingsMap; // Instance property to record all transient data
  public storedPodData: RunData = [];
  // private _timestamp: number;

  private constructor(initialData: ReadingsMap) {
    this._data = initialData;
    // Initialise pod data storage object
    // Indicies represent timesteps
    this.storedPodData.push(this._data);
  }

  /**
   * Creates new instance of this class, only one will be used throughout runtime
   *   and shared by both data-gen and pod.behaviour files
   * @param initialConditions initial values for all sensor readings
   * @returns new instance
   */
  public static getInstance(initialConditions: ReadingsMap): DataManager {
    if (!DataManager.instance) {
      DataManager.instance = new DataManager(initialConditions);
    }
    return DataManager.instance;
  }

  get data(): ReadingsMap {
    return this._data;
  }

  /**
   * Updates the instance's data with an object containing the new values at each iteration
   * Also records value for each sensor in the isntance's stored data object
   * @param newData current iteration of sensor data object
   */
  set data(newData: ReadingsMap) {
    this._data = newData;
    this.storedPodData.push(newData);
    // To do: Add functionality to upload or send the data to the GUI so it can be viewed in real time
  }
}

/**
 * Thursday discussion:
 * Main issue: the sensors read at different times, so the data is not currently synchronised
 * Also, we want the graph to display in real time, not instantaneously
 * Therefore we need to implement a time delay using setInterval, with the delay being the sensor's sampling time
 * 
 * MQTT upload: data is uploaded as a key-value pair. David said it could potentially be an object, but we could just
 * upload the data of every reading for every sensor at every time step, i.e. whenever getData() is called for any of
 * the N units under any of the 8 sensor types, it's immediately uploaded. While technically this isn't uploading all at
 * once at a given time step, it's literal microseconds apart and makes no difference in practice, considering the purpose
 * of this program is just for testing.
 * 
 * We also discussed that this file, DataManager, may not be necessary, as the data could be uploaded directly from the
 * getData function calls. This file is an additional complication that may not be necessary. Transfer the relevant code
 * into main or sensors, then remove this file.
 * 
 * For instance, the pressure type of sensor might have a sampling interval of 200ms, while the motion type of sensor could
 * be 500ms. Therefore, the pressure sensor would be read more frequently than the motion sensor, and there is only overlap
 * once every five pressure gauge readings. The pressure gauge calculation involves temperature and/or velocity, so there are
 * two options: either calculate the temp/motion value at that time step to use in determining the pressure, then only upload
 * the pressure gauge value, or estimate temp/velocity using linear interpolation, which is perfectly fine for these small time
 * intervals, not to mention the logic itself is already an estimation of the real function of time that these readings will
 * follow.
 * 
 * Currently, the getData function is called for every, say, accelerometer, even though they're identical. Thermistors are a little
 * different, as they will likely be a slight temperature gradient throughout the pod, but the pod itself is a rigid body so
 * all of it moves at the same velocity (and by extension experiences the same acceleration). Therefore, we could just have one
  * accelerometer reading and one thermistor reading, as the logic to get each one's value is identical (albeit the values will differ
  * due to random noise). However, I don't necessarily agree as the thermistor readings will be different at different points in the
  * pod in reality, and not only that but the current setup allows us to take the average of the sensor readings to get temperature,
  * reducing noise through averaging. For motion, I'd be happier to just have one reading, as the MQTT only receives one value for
  * acceleration anyway. But with the thermistors, there's no 'temperature' reading, as they individually measure different temperatures 
  * by nature of temperature differences. It's the converse to an isothermal system of multiple moving masses, where we'd need all
  * motion readings, but only one temperature reading.
  * 
  * We don't necessarily need a stored data object like the one here. The data is displayed on the graph, and it's just a simulation
  * so there's no need to review all generated data for a run. If needed, we can just add it for a test run.
  * 
  * KEYENCE: it's a sensor that measures the number of stripes on a surface. It's a digital sensor, so it's not a float, it's an integer.
  * It literally just counts the physical poles the pod passes, it's an optical sensor. It's directly proportional to the pod displacement,
  * which means its range of [0,16] stripes corresponds to displacement range of [0,100] m. It has no noise, of course. Its logic
  * is simple - take the displacement and take the Math.floor of it divided by 16. E.g. floor(53m/16) = 3 stripes. Its graph will show
  * a horizontal line, with steps every 16m. So the class only needs to extend Motion.
  * 
  * Create some sort of time tracking class or dynamic object, kind of like datamanager, so all getData calls can see if the other
  * readings they use for their calculations are up to date. If not, calculate the others. This is a bit of a mess, as it's not a 
  * straightforward dependency graph. It's more like a web, where each sensor depends on a few others, and those depend on a few others,
  * and so on. It's not a tree, it's a graph. It's not a DAG, it's a graph. It's not a directed graph, it's a graph. It's not a cyclic
  * graph, it's a graph. It's not a planar graph, it's a graph. It's not a complete graph, it's a graph. It's not a bipartite graph, it's
  * a graph. It's not a regular graph, it's a graph. It's not a connected graph, it's a graph. It's not a tree, it's a graph. It's not a
  * ^autocomplete lol
 */