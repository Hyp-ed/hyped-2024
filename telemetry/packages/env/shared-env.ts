import { createEnv } from '@t3-oss/env-core';
import { z } from 'zod';

/**
 * HYPED Telemetry environment variables.
 */
export const sharedEnv = createEnv({
  server: {},

  /**
   * The prefix that client-side variables must have. This is enforced both at
   * a type-level and at runtime.
   */
  clientPrefix: 'PUBLIC_',

  client: {
    PUBLIC_MQTT_BROKER_HOST: z.string().min(1),
    PUBLIC_MQTT_QOS: z.number().int().min(0).max(2),
    PUBLIC_TELEMETRY_SERVER_URL: z.string().min(1),
  },

  /**
   * What object holds the environment variables at runtime. This is usually
   * `process.env` or `import.meta.env`.
   */
  runtimeEnv: process.env,

  /**
   * By default, this library will feed the environment variables directly to
   * the Zod validator.
   *
   * This means that if you have an empty string for a value that is supposed
   * to be a number (e.g. `PORT=` in a ".env" file), Zod will incorrectly flag
   * it as a type mismatch violation. Additionally, if you have an empty string
   * for a value that is supposed to be a string with a default value (e.g.
   * `DOMAIN=` in an ".env" file), the default value will never be applied.
   *
   * In order to solve these issues, we recommend that all new projects
   * explicitly specify this option as true.
   */
  emptyStringAsUndefined: true,

  /**
   * If `true`, the library will not validate the environment variables at
   * runtime. This is useful for tests, but should not be used in production.
   */
  skipValidation: process.env.SKIP_ENV_VALIDATION === 'true',
});
