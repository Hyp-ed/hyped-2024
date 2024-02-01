import { RangeMeasurement } from ".";

// ## Type Relations ## //
/**
 * RangeMeasurement: { name: 'acceleration', key: 'acceleration', unit: 'm/s^2', type: 'motion', format: 'float',
 *      limits: { warning: { low: 0, high: 0 }, critical: { low: -10, high: 10 } }, rms_noise: 0.1, sampling_time: 0.1 }
 *
 * Readings: { acceleration: 2, displacement: 5.3, ... }
*
 * LiveReading: { quantity: 2, readings: Readings & {...RangeMeasurement} }
 * LiveReading: { quantity: 2, readings: { acceleration: 2, displacement: 3 } & {...RangeMeasurement} }
 * 
 * SensorData: { motion: Readings } & { pressure: Readings } & { ... } & ...
 * SensorData: { motion: { acceleration: 2, displacement: 5.3, ... }, pressure: { ... }, ... }
 * 
 * RunData: SensorData[] (array of SensorData objects, indices represent timesteps)
 * RunData: [ { motion: { acceleration: 2, displacement: 5.3, ... }, pressure: { ... }, ... }, { ... }, ... ]
 */


// for the variables representing physical sensors, not derived measurements
// for measurement objects which represent physical sensors
export type LiveReading = Omit<RangeMeasurement, 'name'> & {
    quantity: number;
    readings: Readings;  // measurements it provides; e.g. accelerometer gives values for acceleration, displacement and velocity (latter two indirectly)
}

// sensor's output readings
export type Readings = { 
    [measurement: string]: number 
};

// all sensor's readings data
export type SensorData = {
    [sensor: string]: Readings;
}


export type RunData = SensorData[];



// export type SensorInstance<T extends new (...args: any[]) => any> = InstanceType<T>;
// export type SensorInstance<T extends abstract new (...args: any[]) => any> 
//     = InstanceType<T>;


// combine all sensor data into comprehensive object type to be used throughout data generation
// each array element represents a timestep
/* e.g.
const storedData: RunData = [
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

