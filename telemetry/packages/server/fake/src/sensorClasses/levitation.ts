import { Sensor } from '../baseSensor';
import { Magnetism } from './magnetism';}
import { LiveReading, Readings, utils } from '../index';

export class Levitation extends Magnetism {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {

    if (this.magSetpoint === 0 && Object.values(Sensor.lastReadings.levitation).every(
      (val: number) => val == 0
    )) {
      return Sensor.lastReadings.levitation;
    }

    // if electromagnets are powered on, levitation follows an exponential
    //   growth curve to a setpoint, overshoots it a little, then oscillates
    //   before quickly reaching steady state and maintaining a constant value
    //   with noise-caused fluctuations

    // create this new function in utils. It's an exp(kt) until it reaches the setpoint, then it oscillates
    // then (if lev >= levSetpoint) it changes to the sinusoidal decay. There should be
    // a saved graph on desmos with the parameters.

    const levEst = utils.logistic(
      t,
      this.levSetpoint,
      0.4,
      t_0
    )

    return {};
  }
}
