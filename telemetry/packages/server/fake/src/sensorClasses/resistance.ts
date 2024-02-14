import { Sensor } from "../baseSensor";
import { Temperature } from "./temperature";
import { LiveReading, Readings, Utilities } from "../../index";

export class Resistance extends Temperature {

    private alpha = 5 * 10**(-3); // Temperature coefficient of resistance (steel)
    private r_init: number;

    /**
     * Instantiates a resistance sensor
     * @param data sensor data for resistance type
     * @param alpha temperature coefficient of resistance
     */
    constructor(data: LiveReading) {
      super(data);
      // Set initial (reference) resistance
      // When instantiated, the lastReadings will be the predefined initial values
      this.r_init = Sensor.lastReadings.resistance.power_line_resistance;
    }
    
    /**
     * Resistance is generally assunmed to be constant value, however
     *   as it is dicatated by the vibrational motion of the conductor's
     *   molecules, it therefore increases with temperature, which is
     *   how it is modelled here.
     * Each material has a temperature coefficien of resistance indicating
     *   the degree to which resistance changes with a unit change in 
     *   material temperature
     * @param t time in seconds
     */
    getData(t: number): Readings {

      if(!Sensor.isSampled['temperature']) {
        this.temperature = Utilities.average(super.getData(t));
        Sensor.isSampled['temperature'] = true;
      }

      // R = R0 * (1 + α(T - T0))
      const readings = Object.keys(Sensor.lastReadings.resistance).map( key => {
        const r_val = this.r_init * (1 + this.alpha * (this.temperature - this.T_init))
        return [key, r_val + Utilities.gaussianRandom(this.rms_noise)]; // add noise
      });

      return Object.fromEntries(readings);
    }

    get tempCoef() {
      return this.alpha;
    }

    set tempCoef(alpha: number) {
      this.alpha = alpha;
    }
}