import { Sensor } from "../baseSensor";
// individual sensor classes
import { Motion } from "./motion";
import { Keyence } from "./keyence";
import { Pressure } from "./pressure";
import { Temperature } from "./temperature";
import { Resistance } from "./resistance";
import { Magnetism } from "./magnetism";
import { Levitation } from "./levitation";

// Package all sensors into one object to be iterated through easily in SensorManager
interface sensorObj<T extends Sensor> {
    [sensor: string]: T;
}
  
export const sensors = {
    motion: Motion,
    keyence: Keyence,
    temperature: Temperature,
    resistance: Resistance,
    pressure: Pressure,
    magnetism: Magnetism,
    levitation: Levitation,
} as sensorObj<any>;