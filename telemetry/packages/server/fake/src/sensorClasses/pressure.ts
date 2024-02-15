import { Sensor } from '../baseSensor';
import { LiveReading, Readings } from '../../index';

export class Pressure extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    return {};
  }
}
