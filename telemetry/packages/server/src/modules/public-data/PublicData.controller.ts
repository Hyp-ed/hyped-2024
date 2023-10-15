import { Controller, Get, Param, Query } from '@nestjs/common';
import { PublicDataService } from './PublicData.service';
import { HistoricalTelemetryDataService } from '../openmct/data/historical/HistoricalTelemetryData.service';

@Controller('pods/:podId/public-data')
export class PublicDataController {
  constructor(
    private publicDataService: PublicDataService,
    private historialTelemetryDataService: HistoricalTelemetryDataService,
  ) {}

  @Get('launch-time')
  async getLaunchTime(@Param('podId') podId: string) {
    return this.publicDataService.getLaunchTime(podId);
  }

  @Get('state')
  async getState(@Param('podId') podId: string) {
    return this.publicDataService.getState(podId);
  }

  @Get('velocity')
  async getData(
    @Param('podId') podId: string,
    @Query('start') startTimestamp: string,
    @Query('end') endTimestamp?: string,
  ) {
    return this.historialTelemetryDataService.getHistoricalReading(
      podId,
      'velocity',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }

  @Get('displacement')
  async getDisplacement(
    @Param('podId') podId: string,
    @Query('start') startTimestamp: string,
    @Query('end') endTimestamp?: string,
  ) {
    return this.historialTelemetryDataService.getHistoricalReading(
      podId,
      'displacement',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }

  @Get('levitation-height')
  async getLevitationHeight(
    @Param('podId') podId: string,
    @Query('start') startTimestamp: string,
    @Query('end') endTimestamp?: string,
  ) {
    return this.historialTelemetryDataService.getHistoricalReading(
      podId,
      'lecitation_height',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }
}
