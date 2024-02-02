import { Sensor, LiveReading, Readings } from '../src';
import { Magnetism } from '.';

export class Levitation extends Magnetism {

    // Measured by Time-of-Flight, and indirectly by hall effect sensors
    // Pod lifts off at some threshold speed then will follow a logistic curve until a defined setpoint
    // As well as noise, add some random spikes or down/up trends from the setpoint (seperate to 
    //   random noise, these will be actual gap height fluctuations)
    // When this occurs, the magnetic field strength changes in unison
    //   (i.e. there's no lag time, unlike the other way around)
    // The hall effect has a dynamic setpoint which is linked to the desired gap height
    // The control system adjusts the field strength to get back to the gap height setpoint

    constructor(time_of_flight: LiveReading) {
        super(time_of_flight);
    }
}