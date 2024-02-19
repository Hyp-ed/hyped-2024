import { Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '@/modules/logger/Logger.decorator';
import { pods } from '@hyped/telemetry-constants';
import {
  PiVersionResult,
  PiStatus,
  PiInfo,
  PiVersionStatus,
} from '@hyped/telemetry-types';
import { validatePodId } from '../common/utils/validatePodId';

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
  public async getAllPis(podId: string): Promise<PiInfo[]> {
    this.logger.log(`Getting all info for pis in pod ${podId}`);
    validatePodId(podId);
    const pis = pods[podId].pis;
    return Promise.all(Object.keys(pis).map((piId) => this.getPi(podId, piId)));
  }

  /**
   * Returns a Pi in a pod.
   * @param podId Pod ID to get Pi from
   * @param piId Pi ID to get
   * @returns Pi in the pod
   */
  public async getPi(podId: string, piId: string): Promise<PiInfo> {
    this.logger.log(`Getting info for pi ${piId} in pod ${podId}`);

    validatePodId(podId);
    const pi = pods[podId].pis[piId];

    const status = await this.getPiStatus(podId, piId);

    if (!status) {
      this.logger.warn(`Could not get status of pi ${piId} in pod ${podId}`);
      return {
        ...pi,
        podId,
        versionStatus: 'unknown',
        status: 'offline',
      };
    }

    const hashes = await this.getPiVersion(podId, piId);

    if (!hashes) {
      this.logger.warn(`Could not get version of pi ${piId} in pod ${podId}`);
      return {
        ...pi,
        podId,
        versionStatus: 'unknown',
        status,
      };
    }

    const { binaryHash, configHash } = hashes;

    // Determine status of Pi
    const versionStatus = await this.getPiVersionStatus(binaryHash, configHash);

    return {
      ...pi,
      podId,
      binaryHash,
      configHash,
      status,
      versionStatus,
    };
  }

  /**
   * Gets the version of the Pi using the daemon. Returns the hash of the binary and config, or null if the version could not be found.
   * @param podId Pod ID of Pi
   * @param piId Pi ID to get version of
   * @returns Hash of binary and config, or null if version could not be found
   */
  private async getPiVersion(
    podId: string,
    piId: string,
  ): Promise<PiVersionResult | null> {
    this.logger.log(`Getting pi ${piId} version in pod ${podId}`);

    // TODO: Create socket connection to the daemon - port 48595, will be moved to config later
    // TODO: For testing purposes, run the Python file from the daemon PR and test on localhost
    await this.simulateDelay();

    return null;
  }

  private async getPiStatus(podId: string, piId: string): Promise<PiStatus> {
    this.logger.log(`Getting pi ${piId} status in pod ${podId}`);

    // TODO: Ping the Pi to see if it is online or offline
    await this.simulateDelay();

    return 'online';
  }

  private async getPiVersionStatus(
    binaryHash: string,
    configHash: string,
  ): Promise<PiVersionStatus> {
    // Get the correct hashes. If the hashes are the same, then the Pi is up-to-date.
    const [upToDateBinaryHash, upToDateConfigHash] = await Promise.all([
      this.getUpToDateBinaryHash(),
      this.getUpToDateConfigHash(),
    ]);
    const upToDate =
      binaryHash === upToDateBinaryHash && configHash === upToDateConfigHash;
    return upToDate ? 'up-to-date' : 'out-of-date';
  }

  public async getUpToDateBinaryHash(branch: string = 'master') {
    // TODO: Pull the latest commit from the given branch, build the binary using Docker, and get the hash of the binary
    await this.simulateDelay();

    return 'hash_1234';
  }

  public async getUpToDateConfigHash(branch: string = 'master') {
    // TODO: Pull the latest commit from the given branch and hash the config file
    await this.simulateDelay();

    return 'hash_5678';
  }

  /**
   * Updates Pi with latest binary.
   * Builds/compiles code using Docker and uses SCP to transfer it to the Pi.
   */
  public async updatePiBinary(podId: string, piId: string) {
    this.logger.log(`Updating pi ${piId} binary in pod ${podId}`);

    // TODO: `scp` the new binary to the Pi OR Tom will implement this in the daemon
    await this.simulateDelay();

    return true;
  }

  /**
   * Updates Pi with latest config.
   * Uses SCP to transfer it to the Pi.
   */
  public async updatePiConfig(podId: string, piId: string) {
    this.logger.log(`Updating pi ${piId} in pod ${podId}`);

    // TODO: `scp` the new config to the Pi OR Tom will implement this in the daemon
    await this.simulateDelay();

    return true;
  }

  private async simulateDelay() {
    return new Promise((resolve) => {
      setTimeout(resolve, 2000);
    });
  }
}
