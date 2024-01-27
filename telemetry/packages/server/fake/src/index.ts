export {
  averageLimits,
  compareLimits,
  movingAvg,
  readData,
} from '../utils/helpers';
export {
  Sensor,
  Navigation,
  Levitation,
  // ...
} from './sensors';
export { DataManager } from '../utils/data-manager';

import { pods } from '../../../constants/src';
export const {
  pod_1: { measurements },
} = pods;
// definitions.ts then reduces measurements to unqSensorObj and exports it
export { default as rangeSensors } from '../utils/config';
