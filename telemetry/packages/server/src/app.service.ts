import { Injectable } from '@nestjs/common';
import { exec } from 'child_process';

@Injectable()
export class AppService {
  /**
   * Returns the current branch of the server using child_process.
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
}
