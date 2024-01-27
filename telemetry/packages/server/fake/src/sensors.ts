// import { pods } from "../../../constants/src/pods/pods";
import { LiveMeasurement, Limits, SensorData } from '../../../types/src';
import { DataManager } from '../utils/data-manager';
// import { averageLimits } from "./helpers";

interface NavData {
  // [key: string]: LiveMeasurement;
  displacement: LiveMeasurement;
  velocity: LiveMeasurement;
  acceleration: LiveMeasurement;
}

/**
 * Sensor class, parent class to all different sensors.
 * @type T is the shared method which generates next data point.
 * It's a generic type but the (optional) params must be sensor objects.
 * Some methods to get the next value require knowledge of other sensors'
 *   last reading.
 */
// class Sensor<T extends (...args: LiveMeasurement[]) => number = () => number> {
class Sensor {
  // public timeStep: number; -> put into child class func params instead of public variable
  protected readonly _name: string;
  protected readonly _format: string;
  protected _currentValue: number;
  protected readonly _limits: Limits;
  protected _dt: number;

  // // Navigation quantities
  // public static initialVelocity: number;

  constructor(data: LiveMeasurement, dt: number) {
    const { name, format, limits, currentValue } = data;
    this._name = name;
    this._format = format;
    this._limits = limits;
    this._currentValue = currentValue;
    this._dt = dt;
  }

  generateRandomValue(): void {
    const { high, low } = this._limits.critical;
    // console.log(this._currentValue)
    const range = high - low;
    this._currentValue =
      this._format == 'float'
        ? parseFloat((Math.random() * range + low).toFixed(2))
        : Math.floor(Math.random() * (range + 1)) + low;
  }

  set value(newVal: number) {
    this._currentValue = newVal;
  }

  get value(): number {
    return this._currentValue;
  }
}

const sens = new Sensor(
  {
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
  },
  500,
);

console.log(sens.value);
sens.value = 34;
console.log(sens.value);
console.log(sens.generateRandomValue());

class Levitation extends Sensor {}

// Maybe make three objects of the Nav class for the three variables

// <...motionVars: [
//     disp: LiveMeasurement, vel: LiveMeasurement, accl: LiveMeasurement
// ]) => number>

// Accelerometer
// For the logistic curve, you could take the derivative of vel. Or just calculate accl from vel.
// Params are disp, vel, accl for their current values
class Navigation extends Sensor {
  quantity: string; // the name of the quantity requested
  velocity: LiveMeasurement;
  public static initialVelocity: number;
  public static maxAcceleration: number;

  constructor(
    navData: NavData,
    dt: number,
    quantity: 'displacement' | 'velocity' | 'acceleration',
  ) {
    const { velocity } = navData;
    super(navData[quantity], dt);
    this.quantity = quantity;
    if (!Navigation.initialVelocity || Navigation.maxAcceleration) {
      Navigation.initialVelocity = velocity.currentValue;
      Navigation.maxAcceleration = navData.acceleration.limits.critical.high;
    }
  }

  /**
   * Gets the next value for requested navigation quantity
   * Measurement sensor is the accelerometer
   * Calculates displacement, velocity and acceleration and returns the one requested
   * @param timestep current time
   * @param prevVel velocity at previous timestep to be used in calculation
   * @returns
   */
  getNextValue(timestep: number, prevVel: number = this.currentValue): number {
    console.log('update time:', this.dt);
    // let prevDisp = data.displacement.currentValue;
    // let prevVel = this.data.velocity.currentValue;
    // let prevDiso = data.velocity.currentValue;
    console.log('dt & timestep');
    console.log(this.dt);
    console.log(timestep);
    // let prevAccl = data.acceleration.currentValue;
    const startVel = Sensor.initialVelocity;

    // Logistic function params
    const maxVel = this.limits.critical.high;
    const growthRate = 0.444; // these params ensure accl. doesn't exceed 5
    const timeOfInflection = 15;

    const maxAccl = data.acceleration.limits.critical.high;

    // Logistic equation and kinematics
    const vel =
      (maxVel - Navigation.initialVelocity) /
        (1 + Math.exp(-growthRate * (t - timeOfInflection))) +
      startVel;
    const accl =
      (vel - prevVel) / this.dt >= maxAccl
        ? maxAccl
        : (vel - prevVel) / this.dt;
    const disp =
      data.displacement.currentValue +
      (vel * this.dt + 0.5 * accl * this.dt ** 2);

    return [disp, vel, accl];
  }
}

export {
  Sensor,
  Navigation,
  Levitation, // add the rest
};

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
