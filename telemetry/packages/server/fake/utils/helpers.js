"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.readData = exports.movingAvg = exports.compareLimits = exports.averageLimits = void 0;
var csvParser = require("csv-parser");
var fs = require("fs");
// import { sprintf } from 'sprintf-js' // resolve dependency conflict for formatting output, unless we just write data to GUI
/**
 * Helper func to get arbitrary initial conditions for certain sensors below
 * Uses simple average
 * @param sensor name of the sensor e.g. 'pressure_back_push'
 * */
function averageLimits(data, sensor) {
    var lims = Object.values(data[sensor].limits.critical);
    return lims.reduce(function (acc, c) { return (acc + c) / 2; });
}
exports.averageLimits = averageLimits;
/**
 * Helper function checks if the limits of a sensor's range are equal to another
 * in order to avoid duplicates in rangeReadings array
 * @param range1 a sensor's allowed range of values
 * @param range2 a sensor of the same type's allowed range
 * @returns true if they share the same limits
*/
function compareLimits(range1, range2) {
    return range1.critical.low === range2.critical.low && range1.critical.high === range2.critical.high;
}
exports.compareLimits = compareLimits;
/**
 * Calculates the average of a given data set
 * TODO: modify to allow option for exponential MA
 * @param vals an array of data values
 */
function movingAvg(vals) {
    return vals.reduce(function (acc, c) { return acc + c; });
}
exports.movingAvg = movingAvg;
function readData(file) {
    return new Promise(function (resolve, reject) {
        // Check if the file exists
        if (!fs.existsSync(file)) {
            reject("File '".concat(file, "' not found."));
            return;
        }
        var initialState = {};
        // console.log('Sensor / Measurement\tTime interval between readings\tValue at time = 0')
        // console.log('---------------------------------------------------------------')
        // Read the CSV file using csv-parser
        var stream = fs.createReadStream(file);
        stream
            .pipe(csvParser())
            .on('data', function (row) {
            // Extract values from the CSV row
            var quantity = row.quantity, dt = row.dt, initialVal = row.initialVal;
            initialState[quantity] = {
                dt: parseInt(dt),
                initialVal: parseFloat(initialVal)
            };
            // Print the information
            // console.log(`${quantity} \t-\t ${dt} \t-\t ${initialVal}`);
        })
            .on('end', function () {
            console.log('Finished reading the CSV file.');
            // console.log("Initial state:", initialState)
            !Object.keys(initialState).length && reject('Provided file was empty');
            resolve(initialState);
        })
            .on('error', function (err) {
            reject('Encountered error reading file', err);
            // resolve(0).then( res => res );;
        });
    });
}
exports.readData = readData;
function writeData(file, mods) {
}
