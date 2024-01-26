import { Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '@/modules/logger/Logger.decorator';
import { pods } from '@hyped/telemetry-constants';

@Injectable()
export class PiManagementService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
  ) {}

  /**
   * Returns all Pis in a pod.
   * @param podId Pod ID to get Pis from
   * @returns All Pis in the pod
   */
  public async getAllPis(podId: string) {
    this.logger.log(`Getting all info for pis in pod ${podId}`);
    const pis = pods[podId].pis;
    return Promise.all(Object.keys(pis).map((piId) => this.getPi(podId, piId)));
  }

  /**
   * Returns a Pi in a pod.
   * @param podId Pod ID to get Pi from
   * @param piId Pi ID to get
   * @returns Pi in the pod
   */
  public async getPi(podId: string, piId: string) {
    this.logger.log(`Getting info for pi ${piId} in pod ${podId}`);
    const pi = pods[podId]!.pis[piId];

    // Get version of Pi
    const hashes = await this.getPiVersion(podId, piId);

    if (!hashes) {
      this.logger.warn(`Could not get version of pi ${piId} in pod ${podId}`);
      return pi;
    }

    const { binary, config } = hashes;

    return {
      ...pi,
      binary,
      config,
    };
  }

  /**
   * Updates Pi with latest code.
   * Builds/compiles code using Docker and uses SFTP to transfer it to the Pi.
   */
  public updatePiCode(podId: string, piId: string) {
    this.logger.log(`Updating pi ${piId} code in pod ${podId}`);
    return true;
  }

  /**
   * Updates Pi with latest config.
   * Uses SFTP to transfer it to the Pi.
   */
  public updatePiConfig(podId: string, piId: string) {
    this.logger.log(`Updating pi ${piId} in pod ${podId}`);
    return true;
  }

  /**
   * Gets the version of the Pi using the daemon. Returns the hash of the binary and config, or null if the version could not be found.
   * @param podId Pod ID of Pi
   * @param piId Pi ID to get version of
   */
  private async getPiVersion(
    podId: string,
    piId: string,
  ): Promise<PiVersionResult | null> {
    this.logger.log(`Getting pi ${piId} version in pod ${podId}`);
    // TODOLater: Implement using daemon from PR #51: https://github.com/Hyp-ed/hyped-2024/pull/51
    await new Promise((resolve) => setTimeout(resolve, 1000)); // Simulate delay
    return {
      binary: 'hash_1234',
      config: 'hash_5678',
    };
  }
}

type PiVersionResult = {
  binary: string;
  config: string;
};
