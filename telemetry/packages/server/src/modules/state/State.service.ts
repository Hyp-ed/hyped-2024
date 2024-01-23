import { Inject, Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '@/modules/logger/Logger.decorator';
import { StateUpdate, StateUpdateSchema } from './StateUpdate.types';
import { StateUpdateValidationError } from './errors/MeasurementReadingValidationError';
import { Point } from '@influxdata/influxdb-client';
import { InfluxService } from '@/modules/influx/Influx.service';
import { ALL_POD_STATES, getStateType } from '@hyped/telemetry-constants';
import { MqttService } from 'nest-mqtt';

@Injectable()
export class StateService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
    private influxService: InfluxService,
    @Inject(MqttService) private readonly mqttService: MqttService,
  ) {}

  public async addStateReading(props: StateUpdate) {
    const validatedState = this.validateStateUpdate(props);

    const { podId, value: state, timestamp } = validatedState;
    const stateType = getStateType(state);

    // If we want to add state to the OpenMCT dashboard, we can do it here

    // Then save it to the database
    const point = new Point('state')
      .timestamp(timestamp)
      .tag('podId', podId)
      .tag('stateType', stateType)
      .stringField('state', state);

    try {
      this.influxService.telemetryWrite.writePoint(point);

      this.logger.debug(
        `Added state ${props.podId}: ${props.value} (Type: ${stateType})`,
        StateService.name,
      );
    } catch (e) {
      this.logger.error(
        `Failed to add state ${props.podId}: ${props.value} (Type: ${stateType})`,
        e,
        StateService.name,
      );
    }
  }

  /**
   * Simply sends an MQTT message on the state topic to set the state of a pod.
   * @param podId The ID of the pod
   * @param state The state to set
   */
  public async setPodState(podId: string, state: string) {
    this.logger.log(
      `Setting pod ${podId} to state ${state}`,
      StateService.name,
    );

    this.validateState(state);

    // Send the MQTT message
    this.mqttService.publish(`hyped/${podId}/state`, state);
  }

  /**
   * Validates a state string is a valid state.
   * @param state The state to validate
   */
  private validateState(state: string) {
    if (!(Object.values(ALL_POD_STATES) as string[]).includes(state)) {
      throw new StateUpdateValidationError('Invalid state');
    }
  }

  /**
   * Validates a state update.
   * @param props The state update to validate
   * @returns The validated state update, or throws an error if invalid
   */
  private validateStateUpdate(props: StateUpdate) {
    const result = StateUpdateSchema.safeParse(props);

    if (!result.success) {
      throw new StateUpdateValidationError(result.error.message);
    }

    return result.data;
  }
}
