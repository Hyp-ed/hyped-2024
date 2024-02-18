import { Controller, Get, HttpException, Param, Query } from '@nestjs/common';
import { PublicDataService } from './PublicData.service';
import { HistoricalTelemetryDataService } from '@/modules/openmct/data/historical/HistoricalTelemetryData.service';
import { POD_IDS, pods } from '@hyped/telemetry-constants';

@Controller('pods/:podId/public-data')
export class PublicDataController {
  constructor(
    private publicDataService: PublicDataService,
    private historialTelemetryDataService: HistoricalTelemetryDataService,
  ) {}

  @Get('launch-time')
  async getLaunchTime(@Param('podId') podId: string) {
    this.validatePodId(podId);
    return this.publicDataService.getLaunchTime(podId);
  }

  @Get('state')
  async getState(@Param('podId') podId: string) {
    this.validatePodId(podId);
    return this.publicDataService.getState(podId);
  }

  @Get('velocity')
  async getData(
    @Param('podId') podId: string,
    @Query('start') startTimestamp: string,
    @Query('end') endTimestamp?: string,
  ) {
    if (!startTimestamp) {
      throw new HttpException("Missing 'start' query parameter", 400);
    }
    this.validatePodId(podId);
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
    if (!startTimestamp) {
      throw new HttpException("Missing 'start' query parameter", 400);
    }
    this.validatePodId(podId);
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
    if (!startTimestamp) {
      throw new HttpException("Missing 'start' query parameter", 400);
    }
    this.validatePodId(podId);
    return this.historialTelemetryDataService.getHistoricalReading(
      podId,
      'levitation_height',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }

  private validatePodId(podId: string) {
    if (!(podId in POD_IDS)) {
      throw new HttpException('Invalid pod ID', 400);
    }
  }
}
