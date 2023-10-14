import { Module } from '@nestjs/common';
import { PublicDataController } from './PublicData.controller';
import { PublicDataService } from './PublicData.service';
import { HistoricalTelemetryDataService } from '../openmct/data/historical/HistoricalTelemetryData.service';
import { InfluxModule } from '../influx/Influx.module';

@Module({
  imports: [InfluxModule],
  controllers: [PublicDataController],
  providers: [PublicDataService, HistoricalTelemetryDataService],
})
export class PublicDataModule {}
