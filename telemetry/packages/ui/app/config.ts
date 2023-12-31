import { z } from 'zod';

const envSchema = z.object({
  VITE_SERVER_ENDPOINT: z.string().url(),
  VITE_MQTT_BROKER: z.string(),
  VITE_MQTT_QOS: z.coerce.number().refine(
    (n) => {
      return n === 0 || n === 1 || n === 2;
    },
    {
      message: 'QoS must be 0, 1, or 2',
    },
  ),
  VITE_DISCONNECTED_MESSAGE_DISABLED: z.coerce.boolean().optional(),
});

const result = envSchema.safeParse({
  VITE_SERVER_ENDPOINT: import.meta.env.VITE_SERVER_ENDPOINT,
  VITE_MQTT_BROKER: import.meta.env.VITE_MQTT_BROKER,
  VITE_MQTT_QOS: import.meta.env.VITE_MQTT_QOS,
  VITE_DISCONNECTED_MESSAGE_DISABLED: import.meta.env
    .VITE_DISCONNECTED_MESSAGE_DISABLED,
});

if (!result.success) {
  throw new Error('Missing or invalid environment variables!' + result.error);
}

// Remove the `VITE_` prefix from the object keys and cast the result to the correct type
const config = Object.fromEntries(
  Object.entries(result.data).map(([key, value]) => [
    key.replace('VITE_', ''),
    value,
  ]),
) as {
  [K in keyof typeof result.data as K extends `VITE_${infer P}`
    ? P
    : never]: (typeof result.data)[K];
};

export { config };
