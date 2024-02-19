import { Motion } from './motion';
import { Sensor } from '../baseSensor';
import { LiveReading, Readings, utils } from '../index';

export class Temperature extends Motion {
  protected temperature: number;
  protected temp_init: number;

  constructor(data: LiveReading) {
    super(data);
    // Set initial temperature value upon instantiation
    this.temp_init = utils.average(Object.values(data.readings));
    // Dynamic value used for reference by any sub-class(es)
    this.temperature = this.temp_init;
  }

  getData(t: number): Readings {
    // const readings = // ... insert main main logic here
    // this.temperature = utils.average(readings); // take thermistor values' average
    // return readings; //
    const airFricCoef = 0.001;
    const trackFricCoef = 0.05;
    const heatGenCoef = 0.1;

    let temp = Math.pow(this.velocity, 3) * airFricCoef + this.velocity * heatGenCoef;
    temp += this.velocity < this.levVelocity
      ? Math.pow(this.velocity, 2) * trackFricCoef
      : Math.pow(this.velocity, 2) * (this.levVelocity / this.velocity)**0.5 * trackFricCoef;

    return Object.fromEntries(Object.entries(Sensor.lastReadings.temperature)
      .map(([sensor, value]) => {
        return [sensor, value + temp + utils.gaussianRandom(this.rms_noise)]
      })
    );
  }
}
