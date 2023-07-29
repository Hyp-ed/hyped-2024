import { Controller, Param, Post } from '@nestjs/common';
import { WarningsService } from './Warnings.service';

@Controller('pods/:podId/warnings')
export class WarningsController {
  constructor(private warningsService: WarningsService) {}

  @Post('latency')
  async createLatencyWarning(@Param('podId') podId: string) {
    await this.warningsService.createLatencyWarning(podId);
  }
}
