/*
File to generate series of values from pod's operational thermistor data
Main function to be exported to fake-data.ts which will run all sensor readings
  in tandem  over a set time interval
The function will give the user some agency in how random of a fashion they want the data
  to follow
As well as providing parameters for min, max, mean, initial etc. values
*/


// Arbitrary values for testing purposes
const START_TEMP: number = 20;
const MIN_TEMP: number = 10;
const MAX_TEMP: number = 40;

// const genTempData = (series): number[] => {

//   return 0;
// }

const tInit = 0;
const tFinal = 10 * 10^3; // in milliseconds
const tStep = 

// define array to store temp values
let tempData: number[] = [];

// define the change of the next temp value
let changeDir: 1 | -1;
// Function to generate series of temperature values
const nextTempVal = (
  prevTemp: number,
  tStep: number = 500,
  avgChgPerSec: number = 0.5 // degC
): number => {
  let temp: number = prevTemp;
  // change direction of temp variation if it reaches min or max
  if (prevTemp <= MIN_TEMP) {
    console.log("Temp reached range min boundary")
    console.log("Call a function to adjust pod parameters to increase temp")
    temp = MIN_TEMP; // ensure temp doesn't exceed min
    changeDir = 1;
  } else if (prevTemp >= MAX_TEMP) {
    console.log("Temp reached range max boundary")
    console.log("Call a function to adjust pod parameters to lower temp")
    temp = MAX_TEMP; // ensure temp doesn't exceed max
    changeDir = -1;
  } else {
    // Add an element of randomness to represent noise
    // Usually (80% of the time), temp change will remain in the direction 
    //  it was already changing in
    changeDir = Math.random() > 0.8 ? (changeDir === 1 ? -1 : 1) : changeDir;
    // Temp change will differ depending on timestep
    // Change temp by a random amount in specified direction
    //  with the change proportional to the specific avg. temp change per second
    // The line below generates num from 0 to avg. temp change per second
    //  and offsets change so that the avg is in the centre of the range of 
    //  temp increases it can undergo
    // E.g. prevTemp = 10 -> temp = [0,1] * 0.5˚C/second + 0.5/2 = a change between [0.25, 0.75]˚C
    temp += (Math.random() * avgChgPerSec + (avgChgPerSec / 2)) * changeDir;
    // adjust tempChange in accordance with timestep (i.e. if tStep is 500ms, tempChange is half of
    //  what was calculated per second in line above)
    temp *= (tStep / 1000);
  }
  tempData.push(temp);
  return temp;
}

for (let t = tInit; t < tFinal; t += tStep) {
  
