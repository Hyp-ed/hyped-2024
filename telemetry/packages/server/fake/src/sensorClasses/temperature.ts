import { Motion } from './motion';
import { Sensor } from '../baseSensor';
import { LiveReading, Readings, utils } from '../index';

export class Temperature extends Motion {
  protected temp: number;
  protected temp0: number;

  // Arbitrary coefficients for estimating temperature changes
  private params = {
    drag: 0.1,
    friction: 0.3,
    heatGen: 0.5,
  };

  constructor(data: LiveReading) {
    super(data);
    // Initial temp used for reference by subclass(es)
    this.temp0 = utils.average(Object.values(data.readings));
    this.temp = this.temp0;
  }

  getData(t: number): Readings {
    this.temp += // Air drag and internal heat generation
      Math.pow(this.velocity, 3) * this.params.drag +
      this.velocity * this.params.heatGen;
    this.temp += // On the track, temperature increases with work done
      this.velocity < this.liftoffSpeed
        ? Math.pow(this.displacement, 2) * this.params.friction
        : Math.pow(this.displacement, 2) *
          (this.liftoffSpeed / this.velocity) *
          this.displacement *
          this.params.friction;

    return Object.fromEntries(
      Object.keys(Sensor.lastReadings.temperature).map((key) => {
        return [
          key,
          utils.round2DP(this.temp + utils.gaussianRandom(this.rms_noise)),
        ];
      }),
    );
  }
}
