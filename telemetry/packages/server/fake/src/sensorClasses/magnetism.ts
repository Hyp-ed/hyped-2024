import { Motion } from './motion';
import { Sensor } from '../baseSensor';
import { LiveReading, Readings, utils } from '../index';

export class Magnetism extends Motion {
  protected magSetpoint = 250; // A

  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    if (!Sensor.isSampled['motion']) {
      this.velocity = super.getData(t).velocity;
      Sensor.isSampled['motion'] = true;
    } else {
      this.velocity = Sensor.lastReadings.motion.velocity;
    }

    return Object.fromEntries(
      Object.keys(Sensor.lastReadings.magnetism).map((key) => {
        // binary on or off - instantaneous step change
        return [
          key,
          (this.velocity >= this.liftoffSpeed ? this.magSetpoint : 0) +
            utils.gaussianRandom(this.rms_noise),
        ];
      }),
    );
  }

  /**
   * For subclasses to check if EM field is powered on
   * EM field only switched on when corresponding velocity is detected
   * Hence use of last known velocity value instead of generating a new one
   */
  protected isFieldOn(): boolean {
    this.velocity = Sensor.lastReadings.motion.velocity;
    return this.velocity >= this.liftoffSpeed;
  }
}
