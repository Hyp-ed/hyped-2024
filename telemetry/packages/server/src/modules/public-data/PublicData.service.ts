import { Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '../logger/Logger.decorator';
import { HistoricalTelemetryDataService } from '../openmct/data/historical/HistoricalTelemetryData.service';

@Injectable()
export class PublicDataService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
    private historicalTelemetryDataService: HistoricalTelemetryDataService,
  ) {}

  public async getVelocity(
    podId: string,
    startTimestamp: string,
    endTimestamp?: string,
  ) {
    return this.historicalTelemetryDataService.getHistoricalReading(
      podId,
      'velocity',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }

  public async getDisplacement(
    podId: string,
    startTimestamp: string,
    endTimestamp?: string,
  ) {
    return this.historicalTelemetryDataService.getHistoricalReading(
      podId,
      'displacement',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }
}
