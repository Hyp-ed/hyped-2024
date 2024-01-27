import { RangeMeasurement, LiveMeasurement, SensorData } from "../../../types/src";
import { measurements, averageLimits, readData } from "../src/index";
const path = require("path");

// console.log('\n\n')
// console.log(Object.fromEntries(Object.entries(measurements)))
// console.log('\n\n')
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
export const unqSensorObj: SensorData = Object.fromEntries( Object.entries(measurements)
    .map( (sensor): [string, RangeMeasurement] | null => {
        const substrSlice = (name: string): [number, number] => [0, name.lastIndexOf('_')];
        // let temp = sensor[0];
        sensor[0] = sensor[0].match(/(\d|avg\b)$/) 
        ? sensor[0].substring(...substrSlice(sensor[0]))
        : sensor[0];
        // if (temp.match(/(\d|avg\b)$/)) {console.log('Sensor not range :', temp.substring(...substrSlice(temp)))}
        // console.log(sensor[0])
        return sensor[1].hasOwnProperty('limits') 
            ? [sensor[0], { ...sensor[1], currentValue: 0, timestep: 500 } ] as [string, LiveMeasurement] : null;
    }).filter((sensor): sensor is [string, LiveMeasurement] => sensor !== null)
)

// TODO create writeData helper function so user can check and modify initial conditions and timestep

const absPath = path.join(__dirname, '../src/sensorData.csv')

// Import initial values into sensor object
readData(absPath)
    .then( v => {
        for (const sens in unqSensorObj) {
            unqSensorObj[sens].currentValue = v[sens].currentValue
        }
    })
    // .then(() => console.log(unqSensorObj))
    .catch( e => {
        console.error(e)
    })