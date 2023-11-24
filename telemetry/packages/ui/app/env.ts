import { z } from 'zod';

const envSchema = z.object({
  PUBLIC_MQTT_BROKER: z.string(),
  PUBLIC_MQTT_QOS: z.coerce.number().refine(
    (n) => {
      return n === 0 || n === 1 || n === 2;
    },
    {
      message: 'QoS must be 0, 1, or 2',
    },
  ),
  PUBLIC_DISCONNECTED_MESSAGE_DISABLED: z.coerce.boolean().optional(),
});

const result = envSchema.safeParse({
  PUBLIC_MQTT_BROKER: process.env.PUBLIC_MQTT_BROKER,
  PUBLIC_MQTT_QOS: process.env.PUBLIC_MQTT_QOS,
  PUBLIC_DISCONNECTED_MESSAGE_DISABLED:
    process.env.PUBLIC_DISCONNECTED_MESSAGE_DISABLED || false,
});

if (!result.success) {
  throw new Error('Missing or invalid environment variables!');
}

// Remove the `PUBLIC_` prefix from the object keys and cast the result to the correct type
const env = Object.fromEntries(
  Object.entries(result.data).map(([key, value]) => [
    key.replace('PUBLIC_', ''),
    value,
  ]),
) as {
  [K in keyof typeof result.data as K extends `PUBLIC_${infer P}`
    ? P
    : never]: (typeof result.data)[K];
};

export { env };
