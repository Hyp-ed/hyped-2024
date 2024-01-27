"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (Object.prototype.hasOwnProperty.call(b, p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        if (typeof b !== "function" && b !== null)
            throw new TypeError("Class extends value " + String(b) + " is not a constructor or null");
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
exports.Levitation = exports.Navigation = exports.Sensor = void 0;
/**
 * Sensor class, parent class to all different sensors.
 * @type T is the shared method which generates next data point.
 * It's a generic type but the (optional) params must be sensor objects.
 * Some methods to get the next value require knowledge of other sensors'
 *   last reading.
 */
// class Sensor<T extends (...args: LiveMeasurement[]) => number = () => number> {
var Sensor = /** @class */ (function () {
    // // Navigation quantities
    // public static initialVelocity: number;
    function Sensor(data, dt) {
        var name = data.name, format = data.format, limits = data.limits, currentValue = data.currentValue;
        this._name = name;
        this._format = format;
        this._limits = limits;
        this._currentValue = currentValue;
        this._dt = dt;
    }
    Sensor.prototype.generateRandomValue = function () {
        var _a = this._limits.critical, high = _a.high, low = _a.low;
        // console.log(this._currentValue)
        var range = high - low;
        this._currentValue = this._format == 'float'
            ? parseFloat((Math.random() * range + low).toFixed(2))
            : Math.floor(Math.random() * (range + 1)) + low;
    };
    Object.defineProperty(Sensor.prototype, "value", {
        get: function () {
            return this._currentValue;
        },
        set: function (newVal) {
            this._currentValue = newVal;
        },
        enumerable: false,
        configurable: true
    });
    return Sensor;
}());
exports.Sensor = Sensor;
var sens = new Sensor({
    name: 'Displacement',
    key: 'displacement',
    format: 'float',
    type: 'displacement',
    unit: 'm',
    limits: {
        critical: {
            low: 0,
            high: 100,
        },
    },
    currentValue: 43,
}, 500);
console.log(sens.value);
sens.value = 34;
console.log(sens.value);
console.log(sens.generateRandomValue());
var Levitation = /** @class */ (function (_super) {
    __extends(Levitation, _super);
    function Levitation() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    return Levitation;
}(Sensor));
exports.Levitation = Levitation;
// Maybe make three objects of the Nav class for the three variables
// <...motionVars: [
//     disp: LiveMeasurement, vel: LiveMeasurement, accl: LiveMeasurement
// ]) => number>
// Accelerometer
// For the logistic curve, you could take the derivative of vel. Or just calculate accl from vel.
// Params are disp, vel, accl for their current values
var Navigation = /** @class */ (function (_super) {
    __extends(Navigation, _super);
    function Navigation(navData, dt, quantity) {
        var _this = this;
        var velocity = navData.velocity;
        _this = _super.call(this, navData[quantity], dt) || this;
        _this.quantity = quantity;
        if (!Navigation.initialVelocity || Navigation.maxAcceleration) {
            Navigation.initialVelocity = velocity.currentValue;
            Navigation.maxAcceleration = navData.acceleration.limits.critical.high;
        }
        return _this;
    }
    /**
     * Gets the next value for requested navigation quantity
     * Measurement sensor is the accelerometer
     * Calculates displacement, velocity and acceleration and returns the one requested
     * @param timestep current time
     * @param prevVel velocity at previous timestep to be used in calculation
     * @returns
     */
    Navigation.prototype.getNextValue = function (timestep, prevVel) {
        if (prevVel === void 0) { prevVel = this.currentValue; }
        console.log('update time:', this.dt);
        // let prevDisp = data.displacement.currentValue;
        // let prevVel = this.data.velocity.currentValue;
        // let prevDiso = data.velocity.currentValue;
        console.log('dt & timestep');
        console.log(this.dt);
        console.log(timestep);
        // let prevAccl = data.acceleration.currentValue;
        var startVel = Sensor.initialVelocity;
        // Logistic function params
        var maxVel = this.limits.critical.high;
        var growthRate = 0.444; // these params ensure accl. doesn't exceed 5
        var timeOfInflection = 15;
        var maxAccl = data.acceleration.limits.critical.high;
        // Logistic equation and kinematics
        var vel = (maxVel - Navigation.initialVelocity) / (1 + Math.exp(-growthRate * (t - timeOfInflection))) + startVel;
        var accl = (vel - prevVel) / this.dt >= maxAccl ? maxAccl
            : (vel - prevVel) / this.dt;
        var disp = data.displacement.currentValue + (vel * this.dt
            + 0.5 * accl * Math.pow(this.dt, 2));
        return [disp, vel, accl];
    };
    return Navigation;
}(Sensor));
exports.Navigation = Navigation;
// public dt: number; //
// let mySens = new Sensor<number>({
//             name: 'Displacement',
//             key: 'displacement',
//             format: 'float',
//             type: 'displacement',
//             unit: 'm',
//             limits: {
//               critical: {
//                 low: 0,
//                 high: 100,
//               },
//             },
//             currentValue: 43,
//           }
// )
// console.log(mySens.getValue());
// let myVar = new Navigation({
//     name: 'Displacement',
//     key: 'displacement',
//     format: 'float',
//     type: 'displacement',
//     unit: 'm',
//     limits: {
//       critical: {
//         low: 0,
//         high: 100,
//       },
//     },
//     currentValue: 43,
//   }, 500);
// class Sensor implements LiveMeasurement<number> {
//     constructor
// }
