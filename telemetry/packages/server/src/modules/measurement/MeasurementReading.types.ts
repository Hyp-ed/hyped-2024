import { z } from 'zod';

export const MeasurementReadingSchema = z.object({
  podId: z.string(),
  measurementKey: z.string(),
  timestamp: z.string(), // to handle nanoseconds timestamp
  value: z.number(),
});

export type MeasurementReading = z.infer<typeof MeasurementReadingSchema>;
