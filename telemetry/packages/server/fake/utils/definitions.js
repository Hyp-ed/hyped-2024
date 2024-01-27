"use strict";
var __assign = (this && this.__assign) || function () {
    __assign = Object.assign || function(t) {
        for (var s, i = 1, n = arguments.length; i < n; i++) {
            s = arguments[i];
            for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p))
                t[p] = s[p];
        }
        return t;
    };
    return __assign.apply(this, arguments);
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.unqSensorObj = void 0;
var index_1 = require("../src/index");
var path = require("path");
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
exports.unqSensorObj = Object.fromEntries(Object.entries(index_1.measurements)
    .map(function (sensor) {
    var _a;
    var substrSlice = function (name) { return [0, name.lastIndexOf('_')]; };
    // let temp = sensor[0];
    sensor[0] = sensor[0].match(/(\d|avg\b)$/)
        ? (_a = sensor[0]).substring.apply(_a, substrSlice(sensor[0])) : sensor[0];
    // if (temp.match(/(\d|avg\b)$/)) {console.log('Sensor not range :', temp.substring(...substrSlice(temp)))}
    // console.log(sensor[0])
    return sensor[1].hasOwnProperty('limits')
        ? [sensor[0], __assign(__assign({}, sensor[1]), { currentValue: 0, timestep: 500 })] : null;
}).filter(function (sensor) { return sensor !== null; }));
// TODO create writeData helper function so user can check and modify initial conditions and timestep
var absPath = path.join(__dirname, '../src/sensorData.csv');
// Import initial values into sensor object
(0, index_1.readData)(absPath)
    .then(function (v) {
    for (var sens in exports.unqSensorObj) {
        exports.unqSensorObj[sens].currentValue = v[sens].currentValue;
    }
}).then(function () { return console.log(exports.unqSensorObj); })
    .catch(function (e) {
    console.error(e);
});
// setTimeout(() => {
//   console.log(unqSensorObj)
// }, 1000)
