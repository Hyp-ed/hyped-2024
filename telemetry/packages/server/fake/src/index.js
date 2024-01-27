"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.unqSensorObj = exports.measurements = exports.DataManager = exports.Levitation = exports.Navigation = exports.Sensor = exports.readData = exports.movingAvg = exports.compareLimits = exports.averageLimits = void 0;
var helpers_1 = require("../utils/helpers");
Object.defineProperty(exports, "averageLimits", { enumerable: true, get: function () { return helpers_1.averageLimits; } });
Object.defineProperty(exports, "compareLimits", { enumerable: true, get: function () { return helpers_1.compareLimits; } });
Object.defineProperty(exports, "movingAvg", { enumerable: true, get: function () { return helpers_1.movingAvg; } });
Object.defineProperty(exports, "readData", { enumerable: true, get: function () { return helpers_1.readData; } });
var sensors_1 = require("./sensors");
Object.defineProperty(exports, "Sensor", { enumerable: true, get: function () { return sensors_1.Sensor; } });
Object.defineProperty(exports, "Navigation", { enumerable: true, get: function () { return sensors_1.Navigation; } });
Object.defineProperty(exports, "Levitation", { enumerable: true, get: function () { return sensors_1.Levitation; } });
var data_manager_1 = require("../utils/data-manager");
Object.defineProperty(exports, "DataManager", { enumerable: true, get: function () { return data_manager_1.DataManager; } });
var src_1 = require("../../../constants/src");
exports.measurements = src_1.pods.pod_1.measurements;
// definitions.ts then reduces measurements to unqSensorObj and exports it
var config_1 = require("../utils/config");
Object.defineProperty(exports, "unqSensorObj", { enumerable: true, get: function () { return config_1.unqSensorObj; } });
