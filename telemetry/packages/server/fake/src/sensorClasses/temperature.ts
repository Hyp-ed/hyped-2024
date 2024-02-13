import { Sensor } from '../../index';
import { LiveReading, Readings } from '../../index';

export class Temperature extends Sensor {
    constructor(data: LiveReading) {
      super(data);
    }
  
    getData(t: number): Readings {}
}