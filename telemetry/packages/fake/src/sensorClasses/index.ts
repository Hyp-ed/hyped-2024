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
export type sensorObj = {
  [sensor: string]: typeof Sensor;
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
} as sensorObj;

export type SensorType = 
  typeof Motion | 
  typeof Keyence |
  typeof Pressure |
  typeof Temperature |
  typeof Resistance |
  typeof Magnetism |
  typeof Levitation