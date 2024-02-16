import { Sensor } from '../baseSensor';
import { LiveReading, Readings } from '../../index';

export class Magnetism extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    return {
      hall_effect_1: 193 // placeholder
    };
  }
}
