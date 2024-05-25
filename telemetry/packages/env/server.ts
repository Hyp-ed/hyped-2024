import { createEnv } from '@t3-oss/env-core';
import { z } from 'zod';

/**
 * HYPED Telemetry environment variables.
 */
export const serverEnv = createEnv({
  server: {
    INFLUX_URL: z.string().min(1),
    INFLUX_TOKEN: z.string().min(1),
    INFLUX_ORG: z.string().min(1),
    INFLUX_TELEMETRY_BUCKET: z.string().min(1),
    INFLUX_FAULTS_BUCKET: z.number().int().min(1),
  },
  runtimeEnv: process.env,
  emptyStringAsUndefined: true,
});
