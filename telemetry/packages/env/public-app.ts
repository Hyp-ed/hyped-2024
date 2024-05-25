import { createEnv } from '@t3-oss/env-core';
import { z } from 'zod';

/**
 * HYPED Telemetry environment variables.
 */
export const publicAppEnv = createEnv({
  clientPrefix: 'NEXT_PUBLIC_',
  client: {
    NEXT_PUBLIC_POD_ID: z.string().min(1),
  },
  runtimeEnv: {
    NEXT_PUBLIC_POD_ID: process.env.NEXT_PUBLIC_POD_ID,
  },
  emptyStringAsUndefined: true,
  skipValidation: process.env.SKIP_ENV_VALIDATION === 'true',
});
