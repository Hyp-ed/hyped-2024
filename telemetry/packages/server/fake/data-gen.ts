import { pods } from '../../constants/src/pods/pods';
import type { RangeMeasurement, Measurement, Limits } from '../../../packages/types/src/index';
import { Behaviour } from './pod.behaviour';
import { DataManager } from './utils/data-manager';

// Generate data for 50s in steps of 0.5s
const updateTime = 500;
const startTime = 0
const endTime = 50 * 1000 // 50s

// Reduce sensors object to one with only relevant range-based sensors
const sensors = Object.entries(pods.pod_1.measurements);

/**
 * This setup stage manipulates an array of sensor objects to remove redundancy
 * 
 * Firstly a new property for the current data value is added to each sensor object
 * Then we create an interface which mirrors each sensor object from the nested 
 *   measurements object in pods.ts
 * 
 * The array manipulation makes all duplicates equivalent strings to facilitate filtering
 *      e.g. 'thermistor_1', 'thermistor_2' -> both 'thermistor'
 * As the result is an object, it can only have one 'thermistor' key
 * 
 * The code then removes sensors without a limits property (i.e. those of type EnumMeasurement)
 *      as well as unnecessary duplicates by the Objects' inherent unique key characteristics
 */
interface LiveMeasurement extends RangeMeasurement {
    currentVal: number;
    movingAvg: number;
}
export interface SensorData extends Record<string, LiveMeasurement> {}

const unqSensorObj: SensorData = Object.fromEntries( sensors
    .map( (sensor): [string, RangeMeasurement] | null => {
        const substrSlice = (name: string): [number, number] => [0, name.lastIndexOf('_')];
        // let temp = sensor[0];
        sensor[0] = sensor[0].match(/(\d|avg\b)$/) 
        ? sensor[0].substring(...substrSlice(sensor[0]))
        : sensor[0];
        // if (temp.match(/(\d|avg\b)$/)) {console.log('Sensor not range :', temp.substring(...substrSlice(temp)))}
        // console.log(sensor[0])
        return sensor[1].hasOwnProperty('limits') 
            ? [sensor[0], { ...sensor[1], currentVal: 0, movingAvg: 0 } ] as [string, LiveMeasurement] : null;
    }).filter((sensor): sensor is [string, LiveMeasurement] => sensor !== null)
)


// Set initial conditions, updated throughout runtime
interface InitialConditions {
    [key: string]: number;
}
    
const initialConditions: InitialConditions = {
    // Navigation
    accelerometer: 0, displacement: 0, velocity: 0, acceleration: 0,
    // External Pressure
    pressure_back_pull: 1, pressure_front_pull: 1, pressure_back_push: 1, pressure_front_push: 1,
    // Fluid Reservoir Pressure
    pressure_brakes_reservoir: 5, pressure_active_suspension_reservoir: 5,
    // Brake Pressure
    pressure_front_brake: 1, pressure_back_brake: 1,
    // Miscallaneous
    thermistor: 40, hall_effect: averageLimits('hall_effect'), keyence: 0,
    power_line_resistance: 10, levitation_height: 0
}
for (const sens in unqSensorObj) [
    unqSensorObj[sens].currentVal = initialConditions[sens]
]

/* MAIN DATA GENERATION FUNCTION */

const dataManager = DataManager.getInstance(unqSensorObj); // change DataManager file data type to LiveMeasurement type

/* 
References object in DataManager so only needs to be defined once
TODO: change logic so that DataManager instance can only be altered
  directly throuugh updateData(), not through object references */
const currentData = dataManager.getData();


/**
 * MAIN FUNCTION TO GENERATE FAKE DATA
 * 
 * This function will generate and update the sensor object created above at every
 *   timestep, using methods from the pod's Behaviour class to get each next value
 * 
 * @param random: boolean which determines whether to do a completely random data
 *   generation test, or use logical functions to estimate/predict
 *   next data point, reasonably following physical/mathematical rules
 * @param specific User can input an array of strings representing the variables 
 *   to create a fake data series for
 * Otherwise, defaults to false and all variables will be updated every timestep
*/
const generateDataSeries = (random: boolean = false, specific: false | string[] = false) => {
    for (let t = startTime; t <= 1000; t += updateTime) {
        Behaviour.timestep = t;
        if (random) {
            Behaviour.generateRandomValues(currentData);
            console.log(Object.keys(currentData).map( (sensor) => [sensor, currentData[sensor].currentVal]));
            dataManager.addData(
                Object.keys(currentData).map( (sensor) => [sensor, currentData[sensor].currentVal])
            )
            continue;
        }
        if (!specific) {
            for (const dataCategory in currentData) {
                t == startTime && console.log(dataCategory)
                // console.log(dataCategory);
                switch (dataCategory) {
                    case 'displacement':
                    case 'velocity':
                    case 'acceleration':
                        // const [disp, vel, acc] = Behaviour.motionSensors(currentData)
                        break;
                    case 'pressure':
                        //
                        break;
                    case 'temperature':
                        //
                        break;
                    case 'hall_effect':
                        //
                        break;
                    case 'keyence':
                        //
                        break;
                    case 'power_line_resistance':
                        //
                        break;
                    case 'levitation_height':
                        //
                        break;

                }
            }

            continue;
        }
    }
}


generateDataSeries(true);

console.log("Pod Data:", dataManager.storedPodData)




/**
 * Helper func to get arbitrary initial conditions for certain sensors below
 * Uses simple average
 * @param sensor name of the sensor e.g. 'pressure_back_push'
 * */
function averageLimits(sensor: string): number {
    let lims = Object.values(unqSensorObj[sensor].limits.critical)
    return lims.reduce( (acc, c) => (acc + c)/2);
}
/**
 * Helper function checks if the limits of a sensor's range are equal to another
 * in order to avoid duplicates in rangeReadings array
 * @param range1 a sensor's allowed range of values
 * @param range2 a sensor of the same type's allowed range
 * @returns true if they share the same limits
*/
function compareLimits(range1: Limits, range2: Limits): boolean {
    return range1.critical.low === range2.critical.low && range1.critical.high === range2.critical.high;
}