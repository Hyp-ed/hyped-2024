import { Sensor } from '../baseSensor';
import { LiveReading, Readings, utils } from '../index';

export class Levitation extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    return {};
  }
}
