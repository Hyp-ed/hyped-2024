

export class DataManager {
    private static instance: DataManager;  // Static property to hold the single instance
    private data: SensorData;  // Instance property to hold the shared state

    private constructor() {
        // Initialize data
        // this.data = { /* initial data */ };
    }

    static getInstance(): DataManager {
        if (!DataManager.instance) {
            DataManager.instance = new DataManager();
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