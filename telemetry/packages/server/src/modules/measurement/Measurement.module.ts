import { Module } from '@nestjs/common';
import { MeasurementService } from './Measurement.service';
import { InfluxModule } from '../influx/Influx.module';
import { OpenMCTDataModule } from '../openmct/data/OpenMCTData.module';
import { FaultModule } from '../openmct/faults/Fault.module';

@Module({
  imports: [InfluxModule, OpenMCTDataModule, FaultModule],
  providers: [MeasurementService],
  exports: [MeasurementService],
})
export class MeasurementModule {}
