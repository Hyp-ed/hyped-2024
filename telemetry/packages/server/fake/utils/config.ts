import {
  Measurement,
  RangeMeasurement,
  LiveMeasurement,
  SensorData,
} from '../../../types/src';
import { measurements, averageLimits, readData } from '../src/index';
const path = require('path');

/**
 * rangeFilter filters the imported object of measurements into one suitable for data generation
 * Adds properties required for transient data management defined by the LiveMeasurement type
 * Removes duplicates and EnumMeasurement sensors
 * @param obj a data storage object of measurements
 * @returns filtered object of the same structure as measurements but modified sensor properties
 */
const rangeFilter = (obj: Record<string, Measurement>): SensorData => {
  return Object.fromEntries(
    Object.entries(obj)
      .map((sensor): [string, RangeMeasurement] | null => {
        const substrSlice = (name: string): [number, number] => [
          0,
          name.lastIndexOf('_'),
        ];
        // let temp = sensor[0];
        sensor[0] = sensor[0].match(/(\d|avg\b)$/)
          ? sensor[0].substring(...substrSlice(sensor[0]))
          : sensor[0];
        // if (temp.match(/(\d|avg\b)$/)) {console.log('Sensor not range :', temp.substring(...substrSlice(temp)))}
        // console.log(sensor[0])
        return sensor[1].hasOwnProperty('limits')
          ? ([sensor[0], { ...sensor[1], currentValue: 0, timestep: 500 }] as [
              string,
              LiveMeasurement,
            ])
          : null;
      })
      .filter((sensor): sensor is [string, LiveMeasurement] => sensor !== null),
  );
};

// TODO create writeData helper function so user can check and modify initial conditions and timestep

const absPath = path.join(__dirname, '../src/sensorData.csv');

// Import initial values into sensor object
const rangeSensors = rangeFilter(measurements);
readData(absPath)
  .then((v) => {
    for (const sens in rangeSensors) {
      rangeSensors[sens].currentValue = v[sens].initialVal;
    }
  })
  .then(() => console.log(rangeSensors))
  .catch((e) => {
    console.error(e);
  });

export default rangeSensors;
