import { rangeSensors, DataManager } from './src';
import {
  Navigation,
  Levitation,
  Sensor,
  // ...
} from './src';
import { Behaviour } from './pod.behaviour'; // replace with import Sensor classes
import { LiveMeasurement, SensorData } from '../../types/src';

// console.log(Sensors);
// console.log(unqSensorObj);

// Generate data for 50s in steps of 0.5s
export const updateTime = 250;
const startTime = 0;
// const startVelocity = unqSensorObj.velocity.currentValue;
// Sensors.Navigation.initialVelocity = startVelocity;

// const displacement = Sensors.Navigation()

/**
 * Make instances of all the categores and finish making the classes in sensors.ts
 * E.g. const nav = new Navigation(currentData.displacement, etc.)
 * Or you could always just pass in currentData, then in the sensor class just extract
 *  what you need via destructuring
 */

const dataManager = DataManager.getInstance(rangeSensors); // change DataManager file data type to LiveMeasurement type

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
const generateDataSeries = (
  runTime: number,
  random: boolean = false,
  specific: false | string[] = false,
) => {
  const startData: SensorData = JSON.parse(
    JSON.stringify(dataManager.getData()),
  );

  if (random) {
    for (const sensor of Object.values(startData)) {
      console.log(sensor);
    }
    return;
  }

  // Behaviour.dt = updateTime / 1000; // set time interval used for time-dependent calculations

  for (let t = startTime + updateTime; t <= endTime; t += updateTime) {
    /* Create a deep copy so as not to reference the object in memory
        The data will only be updated once each iteration is complete
        This is so that certain functions won't reference other readings from a future timestep
        E.g. a lot are functions of velocity, so velocity in the data manager instance must 
            not change until all data is generated for a given timestep */
    // Behaviour.timestep = t / 1000; // to use units of seconds in calcs
    // if (random) {
    //     const genSensor = new Sensor(currentData)
    //     Behaviour.generateRandomValues(currentData);
    //     console.log(currentData.acceleration)
    //     dataManager.updateData(currentData);
    //     continue;
    // } else if (!specific) {
    //     console.log('specific', t);
    //     // ### NAVIGATION DATA ### //
    //     const [disp, vel, accl] = Behaviour.motionSensors(currentData);
    //     console.log(`Time: ${t / 1000}, Disp: ${disp}, Vel: ${vel}, Accl: ${accl}\n`);
    //     // ### LEVITATION GAP HEIGHT ### //
    //     // const height = Behaviour.levitationHeight(currentData)
    //     // ### MORE SENSORS ... ### //
    //     // console.log(currentData.displacement)
    //     currentData.displacement.currentValue = disp;
    //     currentData.velocity.currentValue = vel;
    //     currentData.acceleration.currentValue = accl;
    //     // currentData.levitation_height.currentValue = height;
    //     dataManager.updateData(currentData);
    // continue;
    // dataManager.addData([
    //     ['displacement', disp],
    //     ['velocity', vel],
    //     ['acceleration', accl],
    // ]);
    // }
  }
};

// const generateDataSeries = (random: boolean = false, specific: false | string[] = false) => {

//     Behaviour.dt = updateTime / 1000; // set time interval used for time-dependent calculations

//     for (let t = startTime + updateTime; t <= endTime; t += updateTime) {
//         /* Create a deep copy so as not to reference the object in memory
//         The data will only be updated once each iteration is complete
//         This is so that certain functions won't reference other readings from a future timestep
//         E.g. a lot are functions of velocity, so velocity in the data manager instance must
//             not change until all data is generated for a given timestep */
//         const currentData: SensorData = JSON.parse(JSON.stringify(dataManager.getData()));
//         Behaviour.timestep = t / 1000; // to use units of seconds in calcs

//         if (random) {
//             Behaviour.generateRandomValues(currentData);
//             console.log(currentData.acceleration)
//             dataManager.updateData(currentData);
//             continue;
//         } else if (!specific) {
//             console.log('specific', t);
//             // ### NAVIGATION DATA ### //
//             const [disp, vel, accl] = Behaviour.motionSensors(currentData);
//             console.log(`Time: ${t / 1000}, Disp: ${disp}, Vel: ${vel}, Accl: ${accl}\n`);

//             // ### LEVITATION GAP HEIGHT ### //
//             // const height = Behaviour.levitationHeight(currentData)

//             // ### MORE SENSORS ... ### //
//             // console.log(currentData.displacement)

//             currentData.displacement.currentValue = disp;
//             currentData.velocity.currentValue = vel;
//             currentData.acceleration.currentValue = accl;
//             // currentData.levitation_height.currentValue = height;

//             dataManager.updateData(currentData);
//             // continue;
//             // dataManager.addData([
//             //     ['displacement', disp],
//             //     ['velocity', vel],
//             //     ['acceleration', accl],
//             // ]);
//         }
//     }
// }

/*
User prompts here or imported from a GUI form to use as function params
For now just hard code them

const runTime: number = prompt("How long should the data generate for (seconds)? ");
const random: boolean = prompt("Would you like the data points to be completely randomised? [y/n] ") == "y" ? true : false;
// prompt for specifc sensors only, put into array

*/
generateDataSeries(50, true);

// console.log("Pod Data:", dataManager.storedPodData)
