import { pods } from '@hyped/telemetry-constants';
import { Pod, RangeMeasurement } from '@hyped/telemetry-types';

/**
 * Gets all of the measurements from the `pods.ts` file that we want to generate data for. (Currently excludes enum measurements)
 * Optionally could include a whitelist/blacklist of measurements to generate data for.
 */
export const measurements = (Object.values(pods) as Pod[]).reduce(
  (acc, pod) => (
    Object.entries(pod.measurements).forEach(([key, measurement]) => {
      if (measurement.format === 'enum') return;
      acc[key] = measurement;
    }),
    acc
  ),
  {} as Record<string, RangeMeasurement>,
);
