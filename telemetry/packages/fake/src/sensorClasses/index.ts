import { Sensor } from '../base';
// Individual sensor classes
import { Motion } from './motion';
import { Keyence } from './keyence';
import { Pressure } from './pressure';
import { Temperature } from './temperature';
import { Resistance } from './resistance';
import { Magnetism } from './magnetism';
import { Levitation } from './levitation';

// Package all sensors into one object to be iterated through easily in SensorManager
interface sensorObj<T extends Sensor> {
  [sensor: string]: T;
}

// Export object containing all sensor classes
export const sensors = {
  motion: Motion,
  keyence: Keyence,
  temperature: Temperature,
  resistance: Resistance,
  pressure: Pressure,
  magnetism: Magnetism,
  levitation: Levitation,
} as sensorObj<any>;
