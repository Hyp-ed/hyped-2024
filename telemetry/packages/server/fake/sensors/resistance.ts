import { Sensor, LiveReading, Readings } from '../src';
import { Temperature } from '.';

export class Resistance extends Temperature {
    readonly initialResistance: number;
    
    constructor(multimeter: LiveReading) {
        super(multimeter);
        this.initialResistance = multimeter.readings.power_line_resistance;
    }

    update(t: number): Readings {
        this.time = t / 1000;
        // Resistance is directly proportional to temperature
        // Use iron resistivity of ~5 * 10^(-3) ohm/K
        this.readings.resistance += (this.readings.temperature - this.initialResistance) * 5 * 10**(-3);
        return this.readings;
    }
}