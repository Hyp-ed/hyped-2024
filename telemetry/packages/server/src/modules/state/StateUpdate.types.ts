import { ALL_POD_STATES, pods } from '@hyped/telemetry-constants';
import {
  zodEnumFromObjKeys,
  zodEnumFromObjValues,
} from '@/modules/common/utils/zodEnumFromObjKeys';
import { z } from 'zod';

export const StateUpdateSchema = z.object({
  podId: zodEnumFromObjKeys(pods),
  timestamp: z.string(), // to handle nanoseconds timestamp
  value: zodEnumFromObjValues(ALL_POD_STATES),
});

export type StateUpdate = z.infer<typeof StateUpdateSchema>;
