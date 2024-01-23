import { SensorData } from "../data-gen";
import { Limits } from "../../../types/src";

interface StoredData {
    [key: string]: (number | string)[]
}


export class DataManager {
    private static instance: DataManager | null = null;  // Static property to hold the single instance
    private data: SensorData;  // Instance property to hold the shared state
    public storedPodData: StoredData = {};
    private limits: Limits; // Instance property to hold sensor reading range limits

    private constructor(data: SensorData) {
        // Initialize data
        this.data = data;
        // Initialise pod data storage object
        for (const sensor in data) {
            this.storedPodData[sensor] = [];
        }
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

    getData(): SensorData {
        return this.data;
    }

    /**
     * Updates the instance's data with an object containing the new values at each iteration
     * Also records value for each sensor in the isntance's stored data object
     * @param newData current iteration of sensor data object
     */
    updateData(newData: SensorData): void {
        this.data = newData;
        Object.keys(newData).forEach( (sensor): void => {
            this.storedPodData[sensor].push(newData[sensor].currentVal);
        });
    }
}