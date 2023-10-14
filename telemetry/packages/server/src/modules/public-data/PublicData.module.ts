import { Module } from '@nestjs/common';
import { PublicDataController } from './PublicData.controller';
import { PublicDataService } from './PublicData.service';
import { HistoricalTelemetryDataService } from '../openmct/data/historical/HistoricalTelemetryData.service';
import { InfluxService } from '../influx/Influx.service';

@Module({
  controllers: [PublicDataController],
  providers: [PublicDataService, HistoricalTelemetryDataService, InfluxService],
})
export class PublicDataModule {}
