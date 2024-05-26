import { POD_IDS, PodId } from '@hyped/telemetry-constants';
import { HttpException } from '@nestjs/common';

/**
 * Validates that the pod ID is valid (exists in the pods object)
 * @param podId Pod ID to validate
 */
export function validatePodId(podId: string): asserts podId is PodId {
  if (!POD_IDS.includes(podId as PodId)) {
    throw new HttpException(`Unknown pod ID: ${podId}`, 400);
  }
}
