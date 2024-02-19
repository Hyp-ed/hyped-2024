import { Motion } from './motion';
import { Sensor } from '../baseSensor';
import { LiveReading, Readings, utils } from '../index';

export class Temperature extends Motion {
  protected temperature: number;
  protected temp_init: number;

  // Coefficients for estimating temperature changes
  private airFricCoef = 0.1;
  private trackFricCoef = 0.3;
  private heatGenCoef = 0.5;

  constructor(data: LiveReading) {
    super(data);
    // Set initial temperature value upon instantiation
    this.temp_init = utils.average(Object.values(data.readings));
    // Dynamic value used for reference by any sub-class(es)
    this.temperature = this.temp_init;
  }

  getData(t: number): Readings {
    let temp = Math.pow(this.velocity, 3) * this.airFricCoef + this.velocity * this.heatGenCoef;
    // While on track, temperature increases with work done
    // After levitation begins, temperature component from track friction gradually falls
    temp += this.velocity < this.levVelocity
      ? Math.pow(this.velocity, 2) * this.trackFricCoef
      : Math.pow(this.velocity, 2) * (this.levVelocity / this.velocity)**0.5 * this.trackFricCoef;

    return Object.fromEntries(Object.entries(
      Sensor.lastReadings.temperature)
        .map(([sensor, value]) => {
          return [sensor, utils.round2DP(value + temp + utils.gaussianRandom(this.rms_noise))]
        })
    );

  }
}
