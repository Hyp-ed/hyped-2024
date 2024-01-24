import { pods } from '../../constants/src/pods/pods';
import type { RangeMeasurement, Measurement, Limits } from '../../../packages/types/src/index';
import { Behaviour } from './pod.behaviour';
import { DataManager } from './utils/data-manager';

// Generate data for 50s in steps of 0.5s
export const updateTime = 250;
const startTime = 0
const endTime = 20 * 1000 // 50s

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
    accelerometer: 0, displacement: 0,
    velocity: unqSensorObj.velocity.limits.critical.high * 0.1, // starting speed 10% of max
    acceleration: 0,
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
dataManager.updateData

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
    
    Behaviour.dt = updateTime / 1000; // set time interval used for time-dependent calculations

    for (let t = startTime + updateTime; t <= endTime; t += updateTime) {
        /* Create a deep copy so as not to reference the object in memory
        The data will only be updated once each iteration is complete
        This is so that certain functions won't reference other readings from a future timestep
        E.g. a lot are functions of velocity, so velocity in the data manager instance must 
            not change until all data is generated for a given timestep */ 
        const currentData: SensorData = JSON.parse(JSON.stringify(dataManager.getData()));
        Behaviour.timestep = t / 1000; // to use units of seconds in calcs

        if (random) {
            Behaviour.generateRandomValues(currentData);
            console.log(currentData.acceleration)
            dataManager.updateData(currentData);
            continue;
        } else if (!specific) {
            console.log('specific', t);
            // ### NAVIGATION DATA ### //
            const [disp, vel, accl] = Behaviour.motionSensors(currentData);
            console.log(`Time: ${t / 1000}, Disp: ${disp}, Vel: ${vel}, Accl: ${accl}\n`);
            
            // ### LEVITATION GAP HEIGHT ### //
            // const height = Behaviour.levitationHeight(currentData)
            
            // ### MORE SENSORS ... ### //
            // console.log(currentData.displacement)
            
            currentData.displacement.currentVal = disp;
            currentData.velocity.currentVal = vel;
            currentData.acceleration.currentVal = accl;
            // currentData.levitation_height.currentVal = height;
            
            
            dataManager.updateData(currentData);
            // continue;
            // dataManager.addData([
            //     ['displacement', disp],
            //     ['velocity', vel],
            //     ['acceleration', accl],
            // ]);
        }
    }
}


generateDataSeries();

console.log("Pod Data:", dataManager.storedPodData)




/**
 * Helper func to get arbitrary initial conditions for certain sensors below
 * Uses simple average
 * @param sensor name of the sensor e.g. 'pressure_back_push'
 * */
export function averageLimits(sensor: string): number {
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