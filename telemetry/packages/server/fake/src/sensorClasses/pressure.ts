import { Sensor } from "../baseSensor";
import { LiveReading, Readings, Utilities } from "../../index";

export class Pressure extends Sensor {
    constructor(data: LiveReading) {
      super(data);
    }
  
    getData(t: number): Readings {}
}