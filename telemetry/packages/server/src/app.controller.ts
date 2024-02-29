import { Controller, Get } from '@nestjs/common';
import { AppService } from './app.service';

@Controller()
export class AppController {
  constructor(private readonly appService: AppService) {}

  @Get('/info/git-branch')
  public async getBranch() {
    return {
      branch: await this.appService.getBranch(),
      uncommittedChanges: await this.appService.getUncommittedChanges(),
    };
  }

  @Get('ping')
  getPing(): string {
    return this.appService.getPing();
  }
}
