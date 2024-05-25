import { createEnv } from '@t3-oss/env-core';
import { z } from 'zod';

/**
 * HYPED Telemetry environment variables for the GUI.
 */
export const uiEnv = createEnv({
  clientPrefix: 'VITE_',
  client: {
    VITE_MQTT_BROKER_HOST: z.string().min(1),
    VITE_MQTT_QOS: z.coerce.number().refine(
      (n) => {
        return n === 0 || n === 1 || n === 2;
      },
      {
        message: 'QoS must be 0, 1, or 2',
      },
    ),
    VITE_UI_HOST: z.string().min(1).default('localhost'),
    VITE_UI_POD_DISCONNECTED_MESSAGES_DISABLED: z.coerce
      .boolean()
      .default(false),
    VITE_HTTP_DEBUG: z.coerce.boolean().default(false),
    VITE_EXTENDED_DEBUGGING_TOOLS: z.coerce.boolean().default(false),
  },
  // @ts-ignore
  runtimeEnv: import.meta.env,
  emptyStringAsUndefined: true,
  skipValidation: process.env.SKIP_ENV_VALIDATION === 'true',
});
