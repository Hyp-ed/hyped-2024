import { pods } from '@hyped/telemetry-constants';
import { HttpException } from '@nestjs/common';

/**
 * Validates that the pod ID is valid (exists in the pods object)
 * @param podId Pod ID to validate
 */
export const validatePodId = (podId: string) => {
  if (pods[podId] === undefined) {
    throw new HttpException(`Unknown pod ID: ${podId}`, 400);
  }
};
