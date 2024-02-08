// External complex types in codebase
export { RangeMeasurement, Limits, Pod } from "../../types/src";
// Complex types defined for live data generation
export { Readings, ReadingsMap, LiveReading, SensorData, RunData, SensorInstance } from "./types"

// Data structure containing all static data on pod telemetry
export { pods } from "../../constants/src/pods/pods";


// Objects & classes created in this directory //

export { measurements, sensorData } from "./env/config"
export { SensorMap } from './src/dataManager';

export { Utilities } from './src/sensorUtils';

// Sensor classes should be encased in an array so as to maintain correct ordering
export * as sensors from './src/sensors';
export { Sensor } from  './src/sensors';
// export { sensorObj };


// export const sensors = [
//   sensorObj.default.Motion,
// ]



// objS;
// console.log(arrS);


// console.log(sensors);
// const a = new sensors[0](sensorData.accelerometer);
// const b = new sensors[1](sensorData.keyence);
// const c = new sensors[3](sensorData.keyence);
// const d = new sensors[5](sensorData.keyence);

// console.log(a.velocity)