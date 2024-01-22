import { pods } from '../../constants/src/pods/pods';
import type { RangeMeasurement, Measurement, Limits } from '../../../packages/types/src/index';
import { Behaviour } from './pod.behaviour';
import { SensorData, DataManager } from './utils/data-manager';

// Generate data for 50s in steps of 0.5s
const deltaT = 500;
const startTime = 0
const endTime = 50 * 1000

// Reduce sensors object to one with only relevant range-based sensors
const sensors = Object.entries(pods.pod_1.measurements);

/**
 * This setup function manipulates array of sensor objects to remove redundancy
 * 
 * It first makes all duplicates equivalent strings to facilitate filtering
 *      e.g. 'thermistor_1', 'thermistor_2' -> both 'thermistor'
 * It then removes those without a limits property (i.e. of type EnumMeasurement)
 *      as well as unnecessary duplicates
 */
const unqSensorObj = sensors
    .map( (sensor): [string, RangeMeasurement] | null => {
        const substrSlice = (name: string): [number, number] => [0, name.lastIndexOf('_')];
        // let temp = sensor[0];
        sensor[0] = sensor[0].match(/(\d|avg\b)$/) 
        ? sensor[0].substring(...substrSlice(sensor[0]))
        : sensor[0];
        // if (temp.match(/(\d|avg\b)$/)) {console.log('Sensor not range :', temp.substring(...substrSlice(temp)))}
        // console.log(sensor[0])
        return sensor[1].hasOwnProperty('limits') 
            ? [sensor[0], sensor[1]] as [string, RangeMeasurement] : null;
    }).filter((sensor): sensor is [string, RangeMeasurement] => sensor !== null
    ).reduce( (acc, [key, val] ) => {
        // console.log(key, val);
        acc[key] = val;
        return acc;
    }, {} as Record<string, RangeMeasurement>);

// console.log(unqSensorObj)

/**
 * Gets arbitrary initial conditions for certain sensors below
 * Uses simple average
 * @param sensor name of the sensor e.g. 'pressure_back_push'
 * */
function averageLimits(sensor: string): number {
    let lims = Object.values(unqSensorObj[sensor].limits.critical)
    return lims.reduce( (acc, c) => (acc + c)/2);
}

// Set as initial conditions, updated throughout runtime
const liveData: SensorData = {
    timestep: 0,
    data: {
        navigation: {
        displacement: 0,
        velocity: 0,
        acceleration: 0,
        },
        pressure: {
            back_pull: 1,
            front_pull: 1,
            back_push: 1,
            front_push: 1,
            brakes_reservoir: 5,
            active_suspension_reservoir: 5,
            front_brake: 1,
            back_brake: 1,
        },
        // Using only one sensor value for those below for fake data generation testing
        // Rest can be added later if needed
        temperature: {
            thermistor: 20,
        },
        hall_effect: {
            // initially set reading to average of limits as it will fluctuate sinusoidally
            hall_effect: averageLimits('hall_effect')
        },
        keyence: {
            keyence: averageLimits('keyence')
        },
        line_resistance: {
            resistance: 10 // closer to min than max, heats up with speed
        },
        levitation_height: {
            height: 0 // rests on track when stationary
        }
    }
}

console.log(liveData);