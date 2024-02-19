import { Sensor } from '../baseSensor';
import { Motion } from './motion';
import { LiveReading, Readings, utils } from '../index';

export class Magnetism extends Motion {

  protected levSetpoint = 50; // mm
  protected magSetpoint = 250; // A

  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number, levCheck?: boolean): Readings {
    if (!Sensor.isSampled['motion']) { 
      this.velocity = super.getData(t).velocity;
      Sensor.isSampled['motion'] = true;
    } else {
      this.velocity = Sensor.lastReadings.motion.velocity;
    }
    
    return Object.fromEntries(Object.keys(
      Sensor.lastReadings.magnetism).map((sensor) => {
        // either off (0 A of current) or on (250 A, an instantaneous step change)
        if (this.isFieldOn()) return [sensor, 0 + utils.gaussianRandom(this.rms_noise)];
        // if (this.velocity < this.levVelocity) {
        //   return [sensor, 0 + utils.gaussianRandom(this.rms_noise)];
        // }
        return [sensor, this.magSetpoint + utils.gaussianRandom(this.rms_noise)];
      })
    );
  }

  // EM powers on when velocity over threshold is detected by sensors, hence not calling 
  //   motion getData method outside of its sampling times
  protected isFieldOn(): boolean {
    this.velocity = Sensor.lastReadings.motion.velocity;
    return this.velocity >= this.levVelocity;
  }
}