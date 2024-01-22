import { Controller, Get, HttpException, Param, Query } from '@nestjs/common';
import { PublicDataService } from './PublicData.service';
import { HistoricalTelemetryDataService } from '@/modules/openmct/data/historical/HistoricalTelemetryData.service';
import { pods } from '@hyped/telemetry-constants';
import {
  LevitationHeightResponse,
  RawLevitationHeight,
} from '@hyped/telemetry-types';

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

  @Get('acceleration')
  async getAcceleration(
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
      'acceleration',
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

    const [
      levitation_height_1,
      levitation_height_2,
      levitation_height_3,
      levitation_height_4,
      levitation_height_lateral_1,
      levitation_height_lateral_2,
    ] = await Promise.all([
      this.historialTelemetryDataService.getHistoricalReading(
        podId,
        'levitation_height_1',
        startTimestamp,
        endTimestamp ?? new Date().getTime().toString(),
      ),
      this.historialTelemetryDataService.getHistoricalReading(
        podId,
        'levitation_height_2',
        startTimestamp,
        endTimestamp ?? new Date().getTime().toString(),
      ),
      this.historialTelemetryDataService.getHistoricalReading(
        podId,
        'levitation_height_3',
        startTimestamp,
        endTimestamp ?? new Date().getTime().toString(),
      ),
      this.historialTelemetryDataService.getHistoricalReading(
        podId,
        'levitation_height_4',
        startTimestamp,
        endTimestamp ?? new Date().getTime().toString(),
      ),
      this.historialTelemetryDataService.getHistoricalReading(
        podId,
        'levitation_height_lateral_1',
        startTimestamp,
        endTimestamp ?? new Date().getTime().toString(),
      ),
      this.historialTelemetryDataService.getHistoricalReading(
        podId,
        'levitation_height_lateral_2',
        startTimestamp,
        endTimestamp ?? new Date().getTime().toString(),
      ),
    ]);

    return {
      levitation_height_1: this.convertValueToInt(levitation_height_1),
      levitation_height_2: this.convertValueToInt(levitation_height_2),
      levitation_height_3: this.convertValueToInt(levitation_height_3),
      levitation_height_4: this.convertValueToInt(levitation_height_4),
      levitation_height_lateral_1: this.convertValueToInt(
        levitation_height_lateral_1,
      ),
      levitation_height_lateral_2: this.convertValueToInt(
        levitation_height_lateral_2,
      ),
    } satisfies LevitationHeightResponse;
  }

  private convertValueToInt(levitationHeights: RawLevitationHeight[]) {
    return levitationHeights.map((reading) => ({
      ...reading,
      value: parseInt(reading.value),
    }));
  }

  private validatePodId(podId: string) {
    if (pods[podId] === undefined) {
      throw new HttpException('Invalid pod ID', 400);
    }
  }
}
