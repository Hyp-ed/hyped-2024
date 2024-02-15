import { Sensor } from '../baseSensor';
import { LiveReading, Readings, Utilities } from '../../index';

export class Temperature extends Sensor {
  protected temperature: number;
  protected T_init: number;

  constructor(data: LiveReading) {
    super(data);
    this.T_init = Utilities.average(data.readings); // initial temperature
    this.temperature = this.T_init; // dynamic value, set to initial temp. upon instantiation
  }

  getData(t: number): Readings {
    // const readings = // ... insert main main logic here
    // this.temperature = Utilities.average(readings); // take thermistor values' average
    // return readings; //
  }
}
