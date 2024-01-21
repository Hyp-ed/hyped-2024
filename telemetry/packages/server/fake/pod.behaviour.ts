import { pods } from '../../constants/src/pods/pods';
import { Limits, Measurement, RangeMeasurement } from '../../types/src/pods/pods.types';

const sensors = Object.entries(pods.pod_1.measurements);

/**
 * This function takes an array of sensor objects and filters them
 * It first removes any without a limits property
 * It then removes any duplicates, e.g. 'thermistor_1', 'thermistor_2'
 */
const uniqueRangeSensors: (string | Measurement)[][] = sensors
    .map( (sensor) => {
        /**
         * Find indices of substring which removes number after sensor name
         * This will avoid runtime redundancy of effectively duplicate sensors
         */
        const substrSlice = (name: string): [number, number] => [0, name.lastIndexOf('_')];

        return sensor[0].match(/\d$/) 
            ? [sensor[0].substring(...substrSlice(sensor[0])), sensor[1]]
            : sensor;

    }).filter( (sensor, i, uniqueRangeSensors) => {
        console.log(i);
        console.log(uniqueRangeSensors[i])
        i > 0 && console.log(sensor[0], uniqueRangeSensors[i - 1][0]);
        return sensor[1].hasOwnProperty('limits')
            ? i == 0 || sensor[0] != uniqueRangeSensors[i - 1][0] : false;
    })

console.log(`Number of unique sensors: ${uniqueRangeSensors.length}`);
// console.log(uniqueRangeSensors);
console.log(uniqueRangeSensors.map(el => el[0]));

/**
 * checks if the limits of a sensor's range are equal to another
 * in order to avoid duplicates in rangeReadings array
 * @param range1 a sensor's allowed range of values
 * @param range2 a sensor of the same type's allowed range
 * @returns true if they share the same limits
 */
function compareLimits(range1: Limits, range2: Limits): boolean {
    return range1.critical.low === range2.critical.low && range1.critical.high === range2.critical.high;
}