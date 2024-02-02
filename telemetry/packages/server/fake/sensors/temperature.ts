import { Sensor, LiveReading, Readings } from '../src';
import { Motion } from '.';

export class Temperature extends Motion {

    constructor(thermistor: LiveReading) {
        super(thermistor);
    }
}