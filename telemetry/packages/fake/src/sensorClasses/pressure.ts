import { Sensor } from '../base';
import { LiveReading, Readings } from '../types';

export class Pressure extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    console.log(t);
    return {
      pressure_front_pull: t, // placeholder
    };
  }
}
