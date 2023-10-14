import { Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '../logger/Logger.decorator';
import { StateUpdate, StateUpdateSchema } from './StateUpdate.types';
import { StateUpdateValidationError } from './errors/MeasurementReadingValidationError';
import { Point } from '@influxdata/influxdb-client';
import { InfluxService } from '../influx/Influx.service';
import { getStateType } from '@hyped/telemetry-constants';

@Injectable()
export class StateService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
    private influxService: InfluxService,
  ) {}

  public async addStateReading(props: StateUpdate) {
    const validatedState = this.validateStateUpdate(props);

    if (!validatedState) {
      throw new StateUpdateValidationError('Invalid state');
    }

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

  private validateStateUpdate(props: StateUpdate) {
    const result = StateUpdateSchema.safeParse(props);
    if (!result.success) {
      throw new StateUpdateValidationError(result.error.message);
    }

    return result.data;
  }
}
