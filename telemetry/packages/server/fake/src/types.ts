import { RangeMeasurement } from ".";

// sensor's output readings
export type Readings = { 
    [measurement: string]: number 
};

// all sensor's readings data
export type SensorData = {
    [sensor: string]: Readings;
}

// for the variables representing physical sensors, not derived measurements
// for measurement objects which represent physical sensors
export type LiveReading = Omit<RangeMeasurement, 'name'> & {
    quantity: number;
    readings: Readings;  // measurements it provides; e.g. accelerometer gives values for acceleration, displacement and velocity (latter two indirectly)
}


// export type SensorInstance<T extends new (...args: any[]) => any> = InstanceType<T>;
export type SensorInstance<T extends abstract new (...args: any[]) => any> 
    = InstanceType<T>;


// combine all sensor data into comprehensive object type to be used throughout data generation
// each array element represents a timestep
export type RunData = SensorData[];
/* e.g.
const liveData: LiveData = [
    {
      motion: { reading1: 2, reading2: 5.3 },
      pressure: { reading1: 5.2, reading2: 4.1, reading3: 0.9 },
      // other sensors...
    },
    {
      motion: { reading1: 3, reading2: 6.3 },
      pressure: { reading1: 6.2, reading2: 5.1, reading3: 1.9 },
      // other sensors...
    },
    // other timesteps...
  ];
*/
