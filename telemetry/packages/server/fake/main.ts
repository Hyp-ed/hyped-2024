import { SensorMap } from './src/sensorManager';

const args = process.argv.slice(2);
const shouldRandomise = args.includes('--random') ? true : false;
const runTime = args.includes('--runtime') ? parseInt(args[args.indexOf('--runtime') + 1]) : 30000;


// Yet to be implemented (also --random could have following args to randomise select sensors)
const sensors = Object.keys(sensorData);
console.log(sensors);

const specificSensors = args.includes('--specific') 
? args.slice(args.indexOf('--specific') + 1).map( (s: string) => s.toLowerCase() ) : false;
// i.e. constructor(... , specific: string[] | false)


// only for testing purposes to avoid having to  CLI input is being developed
import { sensorData } from './env/config';
const allSensors = Object.keys(sensorData);

// Instantiate sensor manager
const sensorMgmt = new SensorMap();

// Instantiate time period
const startTime = Date.now();


/**
 * Main runtime loop function that generates data series and uploads to GUI in real time.
 * @param runTime (CLI) simulation time in ms (not real time, based on sensor timesteps)
 * @param random (CLI) option to simulate random data - later to be replaced with a config object
 * which allows user to randomise select sensor readings. Default is false
 * @param specific (CLI) an array of specific sensor readings to simulate. Default is false 
 * i.e. simulate all sensors
 * @returns 
 */

// if (shouldRandomise) {
//   while (Date.now() - startTime <= runTime) {
//     sensorMgmt.generate();
//   }
// } else {
//   while (Date.now() - startTime <= runTime) {
//     sensorMgmt.generateRandom();
//   }
// }


/**
 * ## To Do ##
 * 
 * [ ] - Implement a function to upload the data to the GUI
 * [ ] - Merge random and default data generation sensorMap methods into one with seperate logic paths
 * 
 * ### Completed ###
 * 
 * [ ] - Import sensorData directly into sensorManager.ts
 * [ ] - Function parameters should be CLI input (and later GUI input)
 * [ ] - samplingTimes and nextSamplingTimes should be defined in SensorMap
 * [ ] - That just leaves the while loop - take it out of the function so it runs immediately
 */


// const samplingTimes = {
//     motion: 1000,
//     pressure: 500,
//     temperature: 500,
//     magnetism: 500,
//     keyence: 500,
//     resistance: 500,
//     levitation: 500
// }