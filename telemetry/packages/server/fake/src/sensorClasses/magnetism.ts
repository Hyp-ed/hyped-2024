import { Sensor } from '../baseSensor';
import { Motion } from './motion';
import { LiveReading, Readings, utils } from '../index';

export class Magnetism extends Motion {

  protected levSetpoint = 50; // mm
  protected magSetpoint = 250; // A

  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    
    return Sensor.lastReadings.magnetism.fromEntries(Object.entries(
      Sensor.lastReadings.magnetism).map(([sensor, value]) => {
        // either off (0 A of current) or on (250 A, an instantaneous step change)
        const val = this.velocity < this.levVelocity ? 0 : this.magSetpoint;
        return [sensor, val + utils.gaussianRandom(this.rms_noise)];
      })
    );
  }
}


// Initial setpoint is 50mm for levitation and 250A for levitation (arbitray, middle of each range)