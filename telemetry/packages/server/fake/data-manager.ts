import { RunData, SensorData } from './src';
import { MQTT_BROKER_HOST } from '@/modules/core/config';
import mqtt from 'mqtt';

export class DataManager {
  private static instance: DataManager | null = null; // Static property to hold the single instance
  private _data: SensorData; // Instance property to record all transient data
  public storedPodData: RunData = [];
  // private _timestamp: number;

  private constructor(initialData: SensorData) {
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
  public static getInstance(initialConditions: SensorData): DataManager {
    if (!DataManager.instance) {
      DataManager.instance = new DataManager(initialConditions);
    }
    return DataManager.instance;
  }

  get data(): SensorData {
    return this._data;
  }

  /**
   * Updates the instance's data with an object containing the new values at each iteration
   * Also records value for each sensor in the isntance's stored data object
   * @param newData current iteration of sensor data object
   */
  set data(newData: SensorData) {
    this._data = newData;
    this.storedPodData.push(this._data)
    // To do: Add functionality to upload or send the data to the GUI so it can be viewed in real time
  }
}
