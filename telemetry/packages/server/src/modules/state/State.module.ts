import { Module } from '@nestjs/common';
import { StateService } from './State.service';
import { InfluxModule } from '../influx/Influx.module';
import { StateController } from './State.controller';

@Module({
  imports: [InfluxModule],
  providers: [StateService],
  exports: [StateService],
  controllers: [StateController],
})
export class StateModule {}
