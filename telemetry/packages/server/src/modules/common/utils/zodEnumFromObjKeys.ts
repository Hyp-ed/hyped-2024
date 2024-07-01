import { z } from 'zod';

export function zodEnumFromObjKeys<K extends string>(
  obj: Record<K, unknown>,
): z.ZodEnum<[K, ...K[]]> {
  const [firstKey, ...otherKeys] = Object.keys(obj) as K[];
  return z.enum([firstKey, ...otherKeys]);
}

export function zodEnumFromObjValues<V extends string>(
  obj: Record<string, V>,
): z.ZodEnum<[V, ...V[]]> {
  const [firstValue, ...otherValues] = Object.values(obj);
  return z.enum([firstValue, ...otherValues]);
}
