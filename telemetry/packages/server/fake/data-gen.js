"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.updateTime = void 0;
var src_1 = require("./src");
// console.log(Sensors);
// console.log(unqSensorObj);
// Generate data for 50s in steps of 0.5s
exports.updateTime = 250;
var startTime = 0;
var endTime = 20 * 1000; // 50s
// const startVelocity = unqSensorObj.velocity.currentValue;
// Sensors.Navigation.initialVelocity = startVelocity;
// const displacement = Sensors.Navigation()
/**
 * Make instances of all the categores and finish making the classes in sensors.ts
 * E.g. const nav = new Navigation(currentData.displacement, etc.)
 * Or you could always just pass in currentData, then in the sensor class just extract
 *  what you need via destructuring
*/
var dataManager = src_1.DataManager.getInstance(src_1.unqSensorObj); // change DataManager file data type to LiveMeasurement type
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
var generateDataSeries = function (random, specific) {
    if (random === void 0) { random = false; }
    if (specific === void 0) { specific = false; }
    var startData = JSON.parse(JSON.stringify(dataManager.getData()));
    if (random) {
        for (var _i = 0, _a = Object.values(startData); _i < _a.length; _i++) {
            var sensor = _a[_i];
            console.log(sensor);
        }
        return;
    }
    // Behaviour.dt = updateTime / 1000; // set time interval used for time-dependent calculations
    for (var t = startTime + exports.updateTime; t <= endTime; t += exports.updateTime) {
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
generateDataSeries(true);
// console.log("Pod Data:", dataManager.storedPodData)
