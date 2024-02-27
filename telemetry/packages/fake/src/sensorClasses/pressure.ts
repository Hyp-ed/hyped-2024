import { Sensor } from '../baseSensor';
import { LiveReading, Readings } from '../types';

export class Pressure extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    return {
      pressure_front_pull: 2.1, // placeholder
    };
  }
}
