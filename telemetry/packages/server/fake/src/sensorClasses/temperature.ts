import { Sensor } from '../baseSensor';
import { LiveReading, Readings, utils } from '../index';

export class Temperature extends Sensor {
  protected temperature: number;
  protected temp_init: number;

  constructor(data: LiveReading) {
    super(data);
    // Set initial temperature value upon instantiation
    this.temp_init = utils.average(Object.values(data.readings));
    // Dynamic value used for reference by any sub-class(es)
    this.temperature = this.temp_init;
  }

  getData(t: number): Readings {
    // const readings = // ... insert main main logic here
    // this.temperature = utils.average(readings); // take thermistor values' average
    // return readings; //
    return {
      thermistor_1: 27.1 // placeholder
    };
  }
}
