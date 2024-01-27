"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.DataManager = void 0;
var DataManager = /** @class */ (function () {
    function DataManager(initialData) {
        this.storedPodData = {};
        // Initialize data
        this.data = initialData;
        // Initialise pod data storage object
        for (var sensor in initialData) {
            this.storedPodData[sensor] = [this.data[sensor].currentValue];
        }
    }
    /**
     * Creates new instance of this class, only one will be used throughout runtime
     *   and shared by both data-gen and pod.behaviour files
     * @param initialConditions initial values for all sensor readings
     * @returns new instance
     */
    DataManager.getInstance = function (initialConditions) {
        if (!DataManager.instance) {
            DataManager.instance = new DataManager(initialConditions);
        }
        return DataManager.instance;
    };
    DataManager.prototype.getData = function () {
        return this.data;
    };
    /**
     * Updates the instance's data with an object containing the new values at each iteration
     * Also records value for each sensor in the isntance's stored data object
     * @param newData current iteration of sensor data object
     */
    DataManager.prototype.updateData = function (newData) {
        var _this = this;
        this.data = newData;
        Object.keys(newData).forEach(function (sensor) {
            _this.storedPodData[sensor].push(newData[sensor].currentValue);
        });
        // add functionality to upload or send the data to the GUI so it can be viewed in real time
    };
    DataManager.instance = null; // Static property to hold the single instance
    return DataManager;
}());
exports.DataManager = DataManager;
