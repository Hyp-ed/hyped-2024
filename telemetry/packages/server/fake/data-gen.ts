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
 * This setup function manipulates array of sensor objects to remove redundancy
 * 
 * It first makes all duplicates equivalent strings to facilitate filtering
 *      e.g. 'thermistor_1', 'thermistor_2' -> both 'thermistor'
 * It then removes those without a limits property (i.e. of type EnumMeasurement)
 *      as well as unnecessary duplicates
 */
interface LiveMeasurement extends RangeMeasurement {
    initialVal: number
}

const unqSensorObj: Record<string, LiveMeasurement> = Object.fromEntries( sensors
    .map( (sensor): [string, RangeMeasurement] | null => {
        const substrSlice = (name: string): [number, number] => [0, name.lastIndexOf('_')];
        // let temp = sensor[0];
        sensor[0] = sensor[0].match(/(\d|avg\b)$/) 
        ? sensor[0].substring(...substrSlice(sensor[0]))
        : sensor[0];
        // if (temp.match(/(\d|avg\b)$/)) {console.log('Sensor not range :', temp.substring(...substrSlice(temp)))}
        // console.log(sensor[0])
        return sensor[1].hasOwnProperty('limits') 
            ? [sensor[0], { ...sensor[1], initialVal: 0 } ] as [string, LiveMeasurement] : null;
    }).filter((sensor): sensor is [string, RangeMeasurement] => sensor !== null)
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
    thermistor: 20, hall_effect: averageLimits('hall_effect'), keyence: 0,
    power_line_resistance: 10, levitation_height: 0
}
for (let sens in unqSensorObj) [
    unqSensorObj[sens].initialVal = initialConditions[sens]
]

console.log("UNIQUE")
console.log(unqSensorObj)

/* MAIN DATA GENERATION FUNCTION */

const dataManager = DataManager.getInstance(unqSensorObj); // change DataManager file data type to LiveMeasurement type
// const podBehaviour = new Behaviour();

const generateDataSeries = () => {
    for (let t = startTime; t <= 2000; t += updateTime) {
        Behaviour.timestep = t;
        const currentData = dataManager.getData();
        // console.log(currentData)
        for (const dataCategory in currentData.data) {
            // console.log(dataCategory);
            switch (dataCategory) {
                case 'navigation':
                    const [disp, vel, acc] = Behaviour.motionSensors(currentData)
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
            for (const measurement in currentData.data[dataCategory]) {
                
                currentData.data[dataCategory][measurement] += 1;
            }
        }
        dataManager.updateData(currentData);
    }
}

generateDataSeries();
// const finalData = dataManager.getData();
// console.log(finalData);
// console.log(Behaviour.timestep);


/**
 * Helper func to get arbitrary initial conditions for certain sensors below
 * Uses simple average
 * @param sensor name of the sensor e.g. 'pressure_back_push'
 * */
function averageLimits(sensor: string): number {
    let lims = Object.values(unqSensorObj[sensor].limits.critical)
    return lims.reduce( (acc, c) => (acc + c)/2);
}