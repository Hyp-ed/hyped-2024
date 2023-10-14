import { ALL_POD_STATES, pods } from '@hyped/telemetry-constants';
import { z } from 'zod';

export const StateUpdateSchema = z.object({
  podId: zodEnumFromObjKeys(pods),
  timestamp: z.string(), // to handle nanoseconds timestamp
  value: zodEnumFromObjKeys(ALL_POD_STATES),
});

export type StateUpdate = z.infer<typeof StateUpdateSchema>;

function zodEnumFromObjKeys<K extends string>(
  obj: Record<K, any>,
): z.ZodEnum<[K, ...K[]]> {
  const [firstKey, ...otherKeys] = Object.keys(obj) as K[];
  return z.enum([firstKey, ...otherKeys]);
}
