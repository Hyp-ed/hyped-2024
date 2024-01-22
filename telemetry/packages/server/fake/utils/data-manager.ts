// import { RangeMeasurement } from "../../../types/src";

import { Limits } from "../../../types/src";

export class DataManager {
    private static instance: DataManager | null = null;  // Static property to hold the single instance
    private data: SensorData;  // Instance property to hold the shared state
    private limits: Limits; // Instance property to hold limits
    public testVar: number = 10;

    private constructor(data: SensorData) {
        // Initialize data
        this.data = data;
    }

    public static getInstance(initialConditions: SensorData): DataManager {
        if (!DataManager.instance) {
            DataManager.instance = new DataManager(initialConditions);
        }
        return DataManager.instance;
    }

    getData(): SensorData {
        return this.data;
    }

    updateData(newData: SensorData): void {
        this.data = newData;
    }
}

export interface SensorData {
    timestep: number;
    data: {
        [key: string]: {
            [key: string]: number
        };
    };
}