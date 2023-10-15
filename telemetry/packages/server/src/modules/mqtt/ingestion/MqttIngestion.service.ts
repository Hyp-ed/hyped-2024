import { Injectable, LoggerService } from '@nestjs/common';
import { Params, Payload, Subscribe } from 'nest-mqtt';
import { MeasurementService } from '@/modules/measurement/Measurement.service';
import { currentTime } from '@influxdata/influxdb-client';
import { StateService } from '@/modules/state/State.service';
import { MqttIngestionError } from './errors/MqttIngestionError';
import { Logger } from '@/modules/logger/Logger.decorator';

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

    this.validateMqttMessage({ podId, measurementKey, value });

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

    this.validateMqttMessage({ podId, measurementKey: 'state', value });

    await this.stateService.addStateReading({
      podId,
      value,
      timestamp,
    });
  }

  private validateMqttMessage({
    podId,
    measurementKey,
    value,
  }: {
    podId: string;
    measurementKey: string;
    value: number;
  }) {
    if (!podId || !measurementKey || value === undefined || isNaN(value)) {
      throw new MqttIngestionError('Invalid MQTT message');
    }
  }
}
