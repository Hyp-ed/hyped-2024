/**
 * Main file which initialises the generation of the data series and uploads to GUI in real time.
 * @param runTime (CLI) simulation time in ms (not real time, based on sensor timesteps)
 * @param random (CLI) option to simulate random data - later to be replaced with a config object
 * which allows user to randomise select sensor readings. Default is false
 * @param specific (CLI) an array of specific sensor readings to simulate. Default is false
 * i.e. simulate all sensors
 */

import { SensorManager } from './src/dataManager';
import { sensorData } from './env/config';

const args = process.argv.slice(2);
const shouldRandomise = args.includes('--random') ? true : false;
const runTime = args.includes('--runtime')
  ? parseInt(args[args.indexOf('--runtime') + 1])
  : 2000;

// If user defined specific sensors, use them, otherwise simulate all sensors
// Essentially setting default value of the array parameter to all sensors
Object.keys(sensorData).filter((sensor) => args.includes(sensor));

// Ensure input sensor options are valid and format them appropriately
const sensorsToRun = args.includes('--specific')
  ? args
      .slice(args.indexOf('--specific') + 1)
      .map((s: string) => s.toLowerCase())
      .filter((s: string) => sensorData.hasOwnProperty(s))
  : Object.keys(sensorData);
// i.e. constructor(... , sensorsToRun: string[])

// Also --random could have following args to randomise select sensors

// Instantiate sensor manager
const sensorMgmt = new SensorManager(sensorsToRun);

// Run data generation simulation for user-defined time period
// This function handles everything from simulationg sensor readings to uploading them to the server
sensorMgmt.generateData(runTime, shouldRandomise);
