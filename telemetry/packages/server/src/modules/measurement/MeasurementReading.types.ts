import { pods } from '@hyped/telemetry-constants';
import { zodEnumFromObjKeys } from 'src/utils/zodEnumFromObjKeys';
import { z } from 'zod';

export const MeasurementReadingSchema = z
  .object({
    podId: zodEnumFromObjKeys(pods),
    measurementKey: z.string(),
    timestamp: z.string(), // to handle nanoseconds timestamp
    value: z.number(),
  })
  // Validate measurement exists
  .refine(
    ({ podId, measurementKey }) => {
      const measurement = pods[podId]['measurements'][measurementKey];
      return measurement;
    },
    {
      message: 'Invalid measurement value',
      path: ['measurementKey'],
    },
  )
  // Validate enum values
  .refine(
    ({ podId, measurementKey, value }) => {
      const measurement = pods[podId]['measurements'][measurementKey];

      if (measurement.format === 'enum') {
        const enumValue = measurement.enumerations.find(
          (e) => e.value === value,
        );

        if (!enumValue) {
          return false;
        }
      }

      return true;
    },
    {
      message: 'Invalid enum value',
      path: ['value'],
    },
  );

export type MeasurementReading = z.infer<typeof MeasurementReadingSchema>;
