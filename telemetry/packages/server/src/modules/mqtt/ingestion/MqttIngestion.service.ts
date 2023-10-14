import { Injectable } from '@nestjs/common';
import { Params, Payload, Subscribe } from 'nest-mqtt';
import { MeasurementService } from '@/modules/measurement/Measurement.service';
import { currentTime } from '@influxdata/influxdb-client';
import { StateService } from '@/modules/state/State.service';
import { isValidState } from '@/modules/state/utils/isValidState';

@Injectable()
export class MqttIngestionService {
  constructor(
    private measurementService: MeasurementService,
    private stateService: StateService,
  ) {}

  @Subscribe('hyped/+/measurement/+')
  async getMeasurementReading(
    @Params() rawParams: string[],
    @Payload() rawValue: any,
  ) {
    const timestamp = currentTime.nanos();
    const podId = rawParams[0];
    const measurementKey = rawParams[1];
    const value = rawValue;

    if (
      !podId ||
      !measurementKey ||
      value === undefined ||
      value === null ||
      isNaN(value)
    ) {
      throw new Error('Invalid MQTT message');
    }

    await this.measurementService.addMeasurementReading({
      podId,
      measurementKey,
      value,
      timestamp,
    });
  }

  @Subscribe('hyped/+/state')
  async getStateReading(
    @Params() rawParams: string[],
    @Payload() rawValue: any,
  ) {
    const timestamp = currentTime.nanos();
    const podId = rawParams[0];
    const value = rawValue;

    if (!podId || value === undefined || value === null) {
      throw new Error('Invalid MQTT message');
    }

    await this.stateService.addStateReading({
      podId,
      value,
      timestamp,
    });
  }
}
