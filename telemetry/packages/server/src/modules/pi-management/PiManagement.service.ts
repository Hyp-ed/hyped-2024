import { Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '@/modules/logger/Logger.decorator';
import { PodId, pods } from '@hyped/telemetry-constants';
import {
  PiConnectionStatus,
  PiInfo,
  VersionStatus,
  Hashes,
  PiId,
} from '@hyped/telemetry-types';
import { validatePodId } from '../common/utils/validatePodId';
import net from 'net';
import isReachable from 'is-reachable';
import fs from 'node:fs';
import { exec, execSync } from 'child_process';
import Client from 'node-scp';

const LOCAL_STORE = 'tmp/binaries';
const TEMP_DIR = 'tmp/build';
const PI_BINARY_DESTINATION = '/home/hyped/hyped';
const PI_CONFIG_DESTINATION = '/home/hyped/config/pod.toml';

@Injectable()
export class PiManagementService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
  ) {}

  /**
   * Returns all Pis in a pod.
   * @param podId Pod ID to get Pis from
   * @param compareBranch Branch to compare the Pi versions to
   * @returns All Pis in the pod
   */
  public async getAllPis(
    podId: string,
    compareBranch: string,
  ): Promise<PiInfo[]> {
    this.logger.log(`Getting all info for pis in pod ${podId}`);
    validatePodId(podId);
    const pis = pods[podId].pis;
    return Promise.all(
      Object.keys(pis).map((piId) => this.getPi(podId, piId, compareBranch)),
    );
  }

  /**
   * Returns a Pi in a pod.
   * @param podId Pod ID to get Pi from
   * @param piId Pi ID to get
   * @param compareBranch Branch to compare the Pi versions to
   * @returns Pi in the pod
   */
  public async getPi(
    podId: string,
    piId: string,
    compareBranch: string,
  ): Promise<PiInfo> {
    this.logger.log(`Getting info for pi ${piId} in pod ${podId}`);

    validatePodId(podId);
    const pi = pods[podId].pis[piId];

    const connectionStatus = await this.getPiStatus(podId, piId);

    if (!connectionStatus) {
      this.logger.warn(`Could not get status of pi ${piId} in pod ${podId}`);
      return {
        ...pi,
        podId,
        connectionStatus,
        binaryHash: null,
        configHash: null,
        binaryStatus: 'unknown',
        configStatus: 'unknown',
      };
    }

    const hashes = await this.getPiHashes(podId, piId);

    const { binaryHash, configHash } = hashes;

    const binaryStatus = binaryHash
      ? await this.getBinaryVersionStatus(binaryHash, compareBranch)
      : 'unknown';
    const configStatus = configHash
      ? await this.getConfigVersionStatus(configHash, compareBranch)
      : 'unknown';

    return {
      ...pi,
      podId,
      connectionStatus,
      binaryHash,
      configHash,
      binaryStatus,
      configStatus,
    };
  }

  /**
   * Gets the version of the Pi using the daemon. Returns the hash of the binary and config, or null if the version could not be found.
   * @param podId Pod ID of Pi
   * @param piId Pi ID to get version of
   * @returns Hash of binary and config, or null if version could not be found
   */
  private async getPiHashes(podId: string, piId: string): Promise<Hashes> {
    this.logger.log(`Getting pi ${piId} version in pod ${podId}`);

    // TODO: Create socket connection to the daemon - port 48595, will be moved to config later
    // TODO: For testing purposes, run the Python file from the daemon PR and test on localhost

    // Create socket
    const socket = new net.Socket();
    const port = 48596; // TODO: Move to config
    const host = 'localhost'; // TODO: Move to config

    // Connect to socket
    socket.connect(port, host);

    // Wait for connection
    try {
      await new Promise((resolve, reject) => {
        socket.on('connect', resolve);
        // give up after 1 seconds
        setTimeout(reject, 1000);
      });
    } catch (e) {
      this.logger.error(`Could not connect to daemon on ${host}:${port}`);
      return {
        binaryHash: null,
        configHash: null,
      };
    }

    // Send message
    socket.write('get_hashes');

    // Wait for response
    const response = await new Promise<
      | {
          result: 'success';
          hyped_pod: string;
          config: string;
        }
      | {
          result: 'error';
          message: string;
        }
    >((resolve) => {
      socket.on('data', (data) => {
        resolve(
          JSON.parse(data.toString()) as
            | {
                result: 'success';
                hyped_pod: string;
                config: string;
              }
            | {
                result: 'error';
                message: string;
              },
        );
      });
    });

    if (response.result === 'error') {
      this.logger.error(
        `Error getting version of pi ${piId} in pod ${podId}: ${response.message}`,
      );
      return {
        binaryHash: null,
        configHash: null,
      };
    }

    const { hyped_pod, config } = response;

    return {
      binaryHash: hyped_pod,
      configHash: config,
    };
  }

  /**
   * Gets the status of a Pi. (Online/Offline)
   * @param podId The pod ID of the Pi
   * @param piId The Pi ID
   * @returns The status of the Pi
   */
  private async getPiStatus(
    podId: PodId,
    piId: string,
  ): Promise<PiConnectionStatus> {
    this.logger.log(`Getting pi ${piId} status in pod ${podId}`);

    const ip = pods[podId].pis[piId].ip;
    const online: unknown = await isReachable(ip, {
      timeout: 1000,
    });

    return online ? 'online' : 'offline';
  }

  /**
   * Gets the status of the binary version on the Pi.
   * @param reportedBinaryHash The hash of the binary reported by the Pi
   * @param compareBranch The branch to compare the binary hash to
   * @returns The status of the binary version (Up-to-date/Out-of-date/Unknown)
   */
  private async getBinaryVersionStatus(
    reportedBinaryHash: string,
    compareBranch: string,
  ): Promise<VersionStatus> {
    // Get the correct hashes. If the hashes are the same, then the Pi is up-to-date.
    const upToDateBinaryHash = await this.getUpToDateBinaryHash(compareBranch);
    const upToDate = reportedBinaryHash === upToDateBinaryHash;
    return upToDate ? 'up-to-date' : 'out-of-date';
  }

  /**
   * Gets the status of the config version on the Pi.
   * @param reportedConfigHash The hash of the config reported by the Pi
   * @param compareBranch The branch to compare the config hash to
   * @returns The status of the config version (Up-to-date/Out-of-date/Unknown)
   */
  private async getConfigVersionStatus(
    reportedConfigHash: string,
    compareBranch: string,
  ): Promise<VersionStatus> {
    // Get the correct hashes. If the hashes are the same, then the Pi is up-to-date.
    const upToDateConfigHash = await this.getUpToDateConfigHash(compareBranch);
    const upToDate = reportedConfigHash === upToDateConfigHash;
    return upToDate ? 'up-to-date' : 'out-of-date';
  }

  /**
   * Gets the hash of the binary on the given branch by building it locally.
   * @param branch The branch to compare the binary hash to
   * @returns The hash of the binary
   */
  public async getUpToDateBinaryHash(
    branch: string = 'master',
  ): Promise<string> {
    this.logger.log(
      `Getting the up-to-date hash of the binary on branch "${branch}"...`,
    );
    return this.buildBinary(branch);
  }

  /**
   * Gets the hash of the config file on the given branch.
   * @param compareBranch The branch to compare the config hash to
   * @returns The hash of the config file
   */
  public async getUpToDateConfigHash(compareBranch: string = 'master') {
    this.logger.verbose(
      `Checking out branch "${compareBranch}" to get the config hash...`,
    );
    await this.checkoutBranch(compareBranch);
    this.logger.verbose('Computing the hash of the config file...');
    return execSync('md5sum config/pod.toml').toString().split(' ')[0];
  }

  /**
   * Updates Pi with latest binary.
   * Builds/compiles code using Docker and uses SCP to transfer it to the Pi.
   * @param podId Pod ID of Pi
   * @param piId Pi ID to update
   * @param branch Branch to update the Pi to
   * @returns `true` if the Pi was updated successfully, `false` otherwise
   */
  public async updatePiBinary(
    podId: PodId,
    piId: PiId,
    branch: string,
  ): Promise<boolean> {
    this.logger.log(
      `Updating pi ${piId} binary in pod ${podId} to the version on branch "${branch}"`,
    );

    this.logger.verbose(
      'Checking if the binary has already been built locally...',
    );

    const commitHash = await this.getLatestBranchCommitHash(branch);

    // Check if we have already built this version of the branch locally
    const binaryPath = `${LOCAL_STORE}/${commitHash}`;

    // If we haven't built this branch yet, build it
    if (!fs.existsSync(binaryPath)) {
      await this.buildBinary(branch);
    }

    try {
      await this.sendFile(podId, piId, binaryPath, PI_BINARY_DESTINATION);
    } catch (e) {
      this.logger.error('Error updating Pi binary');
      return false;
    }

    return true;
  }

  /**
   * Sends a file to a Pi using SCP.
   * @param podId The pod ID of the Pi
   * @param piId The Pi ID
   * @param filePath The path of the file to send
   * @param destination The destination path on the Pi
   * @throws If the file could not be sent
   */
  public async sendFile(
    podId: PodId,
    piId: PiId,
    filePath: string,
    destination: string,
  ) {
    this.logger.log(
      `Transferring file ${filePath} to ${destination} on pi ${piId} in pod ${podId}`,
    );

    const client = await Client({
      host: pods[podId].pis[piId].ip,
      port: 22,
      username: 'hyped',
      password: 'edinburgh',
    });
    await client.uploadFile(filePath, destination);
    client.close();
  }

  /**
   * Builds the binary for the given branch using Docker.
   * @param branch Branch to build the binary for
   * @param localStore Local store for the binaries
   * @returns The hash of the built binary
   */
  public async buildBinary(branch: string): Promise<string> {
    this.logger.verbose(`Building binary for branch "${branch}"...`);

    // Checkout the branch
    await this.checkoutBranch(branch);

    try {
      // Build the cross-compile Docker image
      execSync(
        'docker buildx build -f cc/Dockerfile.crosscompile -t hyped_cc .',
        {
          cwd: TEMP_DIR,
        },
      );
      // Build the binary
      execSync(
        'docker run -e CLEAN=$clean -e DIR=/home/hyped --name hyped_cc -v $(pwd):/home/hyped hyped_cc bash',
        { cwd: TEMP_DIR },
      );
      // Compute the hash of the binary
      const hash = execSync('md5sum build/hyped').toString().split(' ')[0];
      // Get the git commit hash
      const commitHash = await this.getLatestBranchCommitHash(branch);
      // Copy the resulting binary to the local store
      execSync(`cp ${TEMP_DIR}/build/hyped ${LOCAL_STORE}/${commitHash}`);
      return hash;
    } catch (e) {
      this.logger.error('Error building binary');
      throw e;
    }
  }

  /**
   * Gets the latest commit hash of a branch. Used to check whether we need to rebuild from scratch.
   * @param branch The branch to get the latest commit hash of
   * @returns The latest commit hash of the branch
   */
  public async getLatestBranchCommitHash(branch: string) {
    this.logger.verbose(
      `Getting the latest commit hash of branch "${branch}"...`,
    );
    // Make sure we have a clean version of the branch
    await this.checkoutBranch(branch);
    return new Promise<string>((resolve, reject) => {
      exec(
        'git log -n 1 --pretty=format:"%H"',
        { cwd: TEMP_DIR },
        (err, stdout) => {
          if (err) {
            reject(err);
          } else {
            resolve(stdout.trim());
          }
        },
      );
    });
  }

  /**
   * Checks out the given branch in a temporary directory.
   * @param branch The branch to checkout
   * @param tempDir The temporary directory to checkout the branch in
   */
  public async checkoutBranch(branch: string) {
    this.logger.verbose(`Checking out branch "${branch}"...`);

    // Remove the temporary directory if it already exists
    if (fs.existsSync(TEMP_DIR)) {
      fs.rmdirSync(TEMP_DIR, { recursive: true });
    }

    // Create the temporary directory
    fs.mkdirSync(TEMP_DIR, { recursive: true });

    // Checkout the branch
    return new Promise<void>((resolve, reject) => {
      exec(`git checkout ${branch}`, { cwd: TEMP_DIR }, (err) => {
        if (err) {
          reject(err);
        } else {
          resolve();
        }
      });
    });
  }

  /**
   * Updates Pi with latest config.
   * Uses SCP to transfer it to the Pi.
   */
  public async updatePiConfig(podId: PodId, piId: PiId, compareBranch: string) {
    this.logger.log(
      `Updating pi ${piId} config in pod ${podId} to the version on branch "${compareBranch}"`,
    );
    try {
      await this.checkoutBranch(compareBranch);
      await this.sendFile(
        podId,
        piId,
        'config/pod.toml',
        PI_CONFIG_DESTINATION,
      );
    } catch (e) {
      this.logger.error('Error updating Pi config');
      throw e;
    }
  }
}
