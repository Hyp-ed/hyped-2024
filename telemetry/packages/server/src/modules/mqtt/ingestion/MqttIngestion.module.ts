import { Module } from '@nestjs/common';
import { MqttIngestionService } from './MqttIngestion.service';
import { MeasurementModule } from 'src/modules/measurement/Measurement.module';

@Module({
  imports: [MeasurementModule],
  providers: [MqttIngestionService],
})
export class MqttIngestionModule {}
