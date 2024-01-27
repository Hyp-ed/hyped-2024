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
var index_1 = require("../src/index");
var path = require("path");
var rangeFilter = function (obj) {
    return Object.fromEntries(Object.entries(obj)
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
};
// TODO create writeData helper function so user can check and modify initial conditions and timestep
var absPath = path.join(__dirname, '../src/sensorData.csv');
// Import initial values into sensor object
var rangeSensors = rangeFilter(index_1.measurements);
(0, index_1.readData)(absPath)
    .then(function (v) {
    for (var sens in rangeSensors) {
        rangeSensors[sens].currentValue = v[sens].initialVal;
    }
})
    .then(function () { return console.log(rangeSensors); })
    .catch(function (e) {
    console.error(e);
});
exports.default = rangeFilter;
