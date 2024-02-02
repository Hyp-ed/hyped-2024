import { Sensor, LiveReading, Readings } from '../src';
import { Temperature } from '.';

type gaugeType = "pull" | "push" | "reservoir" | "brakes" | "suspension";

export class Pressure extends Temperature {
    
    // pressure loss coefficient * vel^2, from Darcy formula (guesstimate)
    // later define pneumatic tubing parameters (L, D, k)
    protected losses: number = (1.5 * 10**(-5)) * this.velocity ** 2;
    protected gaugeFunction: gaugeType;
    
    constructor(gauge: LiveReading) {
        super(gauge);
    }

    /**
     * Function to categorise the pressure gauges into their respective types and function
    */
    public seperateGauges() {
        console.log(Object.keys(this.readings).forEach( (out: string): void => {
            out.replace(/.*_(.+)/, '$1') as gaugeType;
        }));
    }
    
    /**
     * There are two intake tubes and two exhaust tubes for air, one of each stationed at
     *  the back and front of the pod
     * Assuming there is no other point of infiltration, the air pressure in must be roughly
     *  equal to the air pressure out (assuming minimal losses)
     * Therefor
    */
    public getAirflowPressure() {

    }

    public getReservoirPressure() {

    }

}