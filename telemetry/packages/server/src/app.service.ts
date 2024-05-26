import { Injectable } from '@nestjs/common';
import { exec } from 'child_process';

@Injectable()
export class AppService {
  /**
   * Gets the current branch which the base station is using (i.e. the branch that this code is currently running on).
   * Should be main but could be something else if we're testing something or if we're in a feature branch.
   */
  public getBranch() {
    return new Promise<string>((resolve, reject) => {
      exec('git rev-parse --abbrev-ref HEAD', (err, stdout) => {
        if (err) {
          reject(err);
        } else {
          resolve(stdout.trim());
        }
      });
    });
  }

  /**
   * Checks whether there are uncommitted changes on the current branch.
   * TODO: we probably don't need this because we'll be cloning a temporary
   * repository for the purposes of building?
   * @returns `true` if there are uncommitted changes, `false` otherwise.
   */
  public getUncommittedChanges() {
    return new Promise<boolean>((resolve, reject) => {
      exec('git status --porcelain', (err, stdout) => {
        if (err) {
          reject(err);
        } else {
          resolve(stdout.trim().length > 0);
        }
      });
    });
  }

  /**
   * Gets the commit SHA of the current branch.
   */
  public getCommitSha() {
    return new Promise<string>((resolve, reject) => {
      exec('git rev-parse HEAD', (err, stdout) => {
        if (err) {
          reject(err);
        } else {
          resolve(stdout.trim());
        }
      });
    });
  }

  /**
   * Gets all of the possible branches we could build and deploy to the pis.
   * (Including remote branches, not just local ones.)
   */
  public getAllBranches() {
    return new Promise<string[]>((resolve, reject) => {
      exec('git branch -a --format="%(refname:short)"', (err, stdout) => {
        if (err) {
          reject(err);
        } else {
          resolve(
            stdout
              .split('\n')
              .map((branch) => branch.trim())
              .filter((branch) => branch.length > 0),
          );
        }
      });
    });
  }

  getPing(): string {
    return 'pong';
  }
}
