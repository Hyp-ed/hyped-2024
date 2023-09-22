import { Measurement } from '@hyped/types';
import { MeasurementReading } from '../MeasurementReading.types';
import { RangeMeasurement } from '@hyped/types/dist/pods/pods.types';
import { FaultLevel } from '@hyped/constants';

export type DoesMeasurementBreachLimitsReturn = false | FaultLevel;

export function doesMeasurementBreachLimits(
  measurement: RangeMeasurement,
  reading: MeasurementReading,
): DoesMeasurementBreachLimitsReturn {
  const { value } = reading;

  const { low, high } = measurement.limits.critical;
  if (value < low || value > high) {
    return 'CRITICAL';
  }

  if (measurement.limits.warning) {
    const { low, high } = measurement.limits.warning;

    if (value < low || value > high) {
      return 'WARNING';
    }
  }

  return false;
}
