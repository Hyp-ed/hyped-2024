import { Controller, Get } from '@nestjs/common';
import { AppService } from './app.service';

@Controller()
export class AppController {
  constructor(private readonly appService: AppService) {}

  @Get('/branch')
  public async getBranch() {
    return { branch: await this.appService.getBranch() };
  }
}
