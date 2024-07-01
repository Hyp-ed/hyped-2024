import { Injectable, LoggerService } from '@nestjs/common';
import { Params, Payload, Subscribe } from 'nest-mqtt';
import { MeasurementService } from '@/modules/measurement/Measurement.service';
import { currentTime } from '@influxdata/influxdb-client';
import { StateService } from '@/modules/state/State.service';
import { MqttIngestionError } from './errors/MqttIngestionError';
import { POD_IDS, PodId, PodStateType } from '@hyped/telemetry-constants';
import { Logger } from '@/modules/logger/Logger.decorator';

@Injectable()
export class MqttIngestionService {
  constructor(
    private measurementService: MeasurementService,
    private stateService: StateService,
    @Logger()
    private readonly logger: LoggerService,
  ) {}

  @Subscribe('hyped/+/measurement/+')
  async getMeasurementReading(
    @Params() rawParams: string[],
    @Payload() rawValue: any, // TODOLater: check that this is correct
  ) {
    const timestamp = currentTime.nanos();
    const podId = rawParams[0];
    const measurementKey = rawParams[1];

    // If it's not the cart, do what we normally do
    this.validatePodId(podId);
    if (podId !== 'cart_2024') {
      const value = rawValue as number;
      this.validateMqttMessage({ podId, measurementKey, value });

      await this.measurementService.addMeasurementReading({
        podId,
        measurementKey,
        value,
        timestamp,
      });
    }

    // If it's the cart, add the readings manually
    this.logger.debug(rawValue);

    switch (measurementKey) {
      case 'accelerometer':
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: 'accelerometer_x',
          value: rawValue.x as number,
          timestamp: timestamp,
        });
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: 'accelerometer_y',
          value: rawValue.y as number,
          timestamp: timestamp,
        });
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: 'accelerometer_z',
          value: rawValue.z as number,
          timestamp: timestamp,
        });
        return;
      case 'acceleration':
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: 'acceleration',
          value: rawValue.acceleration as number,
          timestamp: timestamp,
        });
        return;
      case 'velocity':
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: 'velocity',
          value: rawValue.velocity as number,
          timestamp: timestamp,
        });
        return;
      case 'displacement':
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: 'displacement',
          value: rawValue.displacement as number,
          timestamp: timestamp,
        });
        return;
      case 'optical_flow':
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: `optical_flow_x`,
          value: rawValue.x as number,
          timestamp: timestamp,
        });
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: `optical_flow_y`,
          value: rawValue.y as number,
          timestamp: timestamp,
        });
        return;
      case 'keyence':
        await this.measurementService.addMeasurementReading({
          podId: 'cart_2024',
          measurementKey: 'keyence',
          value: rawValue.count as number,
          timestamp: timestamp,
        });
        return;
    }
  }

  @Subscribe('hyped/+/state/state')
  getStateReading(
    @Params() rawParams: string[],
    @Payload()
    rawValue: {
      state: PodStateType;
    },
  ) {
    const timestamp = currentTime.nanos();
    const podId = rawParams[0];
    const value = rawValue.state;

    this.validatePodId(podId);

    this.stateService.addStateReading({
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
    value: unknown;
  }) {
    if (!podId || !measurementKey || value === undefined) {
      throw new MqttIngestionError('Invalid MQTT message');
    }
  }

  private validatePodId(podId: string): asserts podId is PodId {
    if (!POD_IDS.includes(podId as PodId)) {
      throw new MqttIngestionError('Invalid pod ID');
    }
  }
}
