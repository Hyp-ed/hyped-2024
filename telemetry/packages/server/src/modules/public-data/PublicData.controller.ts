import { Controller, Get, Param, Query } from '@nestjs/common';
import { PublicDataService } from './PublicData.service';

@Controller('pods/:podId/public-data')
export class PublicDataController {
  constructor(private publicDataService: PublicDataService) {}

  @Get('velocity')
  async getData(
    @Param('podId') podId: string,
    @Query('start') startTimestamp: string,
    @Query('end') endTimestamp?: string,
  ) {
    return this.publicDataService.getVelocity(
      podId,
      startTimestamp,
      endTimestamp,
    );
  }

  @Get('displacement')
  async getDisplacement(
    @Param('podId') podId: string,
    @Query('start') startTimestamp: string,
    @Query('end') endTimestamp?: string,
  ) {
    return this.publicDataService.getDisplacement(
      podId,
      startTimestamp,
      endTimestamp,
    );
  }

  @Get('state')
  async getState(@Param('podId') podId: string) {
    return this.publicDataService.getState(podId);
  }
}
