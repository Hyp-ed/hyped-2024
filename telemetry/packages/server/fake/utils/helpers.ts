import { SensorData, Limits } from '../../../types/src';
import * as csvParser from 'csv-parser';
import * as fs from 'fs';
import { Readable } from 'stream';
// import { sprintf } from 'sprintf-js' // resolve dependency conflict for formatting output, unless we just write data to GUI

/**
 * Helper func to get arbitrary initial conditions for certain sensors below
 * Uses simple average
 * @param sensor name of the sensor e.g. 'pressure_back_push'
 * */
function averageLimits(data: SensorData, sensor: string): number {
  let lims = Object.values(data[sensor].limits.critical);
  return lims.reduce((acc, c) => (acc + c) / 2);
}

/**
 * Helper function checks if the limits of a sensor's range are equal to another
 * in order to avoid duplicates in rangeReadings array
 * @param range1 a sensor's allowed range of values
 * @param range2 a sensor of the same type's allowed range
 * @returns true if they share the same limits
 */
function compareLimits(range1: Limits, range2: Limits): boolean {
  return (
    range1.critical.low === range2.critical.low &&
    range1.critical.high === range2.critical.high
  );
}

/**
 * Calculates the average of a given data set
 * TODO: modify to allow option for exponential MA
 * @param vals an array of data values
 */
function movingAvg(vals: number[]): number {
  return vals.reduce((acc, c) => acc + c);
}

/**
 * csv read/write function for user input
 */
interface InitialState {
  [key: string]: {
    dt: number;
    initialVal: number;
  };
}

function readData(file: string): Promise<InitialState> {
  return new Promise((resolve, reject) => {
    // Check if the file exists
    if (!fs.existsSync(file)) {
      reject(`File '${file}' not found.`);
      return;
    }

    const initialState: InitialState = {};

    // console.log('Sensor / Measurement\tTime interval between readings\tValue at time = 0')
    // console.log('---------------------------------------------------------------')
    // Read the CSV file using csv-parser

    const stream = fs.createReadStream(file);
    stream
      .pipe(csvParser())
      .on('data', (row) => {
        // Extract values from the CSV row
        const { quantity, dt, initialVal } = row;
        initialState[quantity] = {
          dt: parseInt(dt),
          initialVal: parseFloat(initialVal),
        };

        // Print the information
        // console.log(`${quantity} \t-\t ${dt} \t-\t ${initialVal}`);
      })
      .on('end', () => {
        console.log('Finished reading the CSV file.');
        // console.log("Initial state:", initialState)
        !Object.keys(initialState).length && reject('Provided file was empty');
        resolve(initialState);
      })
      .on('error', (err) => {
        reject('Encountered error reading file', err);
        // resolve(0).then( res => res );;
      });
  });
}

function writeData(file: string, mods: InitialState) {}

export { averageLimits, compareLimits, movingAvg, readData };
