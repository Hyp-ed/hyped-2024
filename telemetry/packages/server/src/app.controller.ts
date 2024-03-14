import { Controller, Get } from '@nestjs/common';
import { AppService } from './app.service';

@Controller()
export class AppController {
  constructor(private readonly appService: AppService) {}

  @Get('/info/git/current-branch')
  public async getBranch() {
    return {
      branch: await this.appService.getBranch(),
      uncommittedChanges: await this.appService.getUncommittedChanges(),
    };
  }

  @Get('/info/git/commit-sha')
  public async getCommitSha() {
    return {
      sha: await this.appService.getCommitSha(),
    };
  }

  @Get('/info/git/all-branches')
  public async getAllBranches() {
    return {
      branches: await this.appService.getAllBranches(),
    };
  }

  @Get('ping')
  getPing(): string {
    return this.appService.getPing();
  }
}
