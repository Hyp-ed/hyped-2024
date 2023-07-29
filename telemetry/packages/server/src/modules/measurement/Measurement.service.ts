import { pods } from '@hyped/telemetry-constants';
import { Point } from '@influxdata/influxdb-client';
import { Injectable, LoggerService } from '@nestjs/common';
import { InfluxService } from '../influx/Influx.service';
import { Logger } from '../logger/Logger.decorator';
import { RealtimeTelemetryDataGateway } from '../openmct/data/realtime/RealtimeTelemetryData.gateway';
import {
  MeasurementReading,
  MeasurementReadingSchema,
} from './MeasurementReading.types';
import { MeasurementReadingValidationError } from './errors/MeasurementReadingValidationError';
import { doesMeasurementBreachLimits } from './utils/doesMeasurementBreachLimits';
import { FaultService } from '../openmct/faults/Fault.service';

@Injectable()
export class MeasurementService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
    private influxService: InfluxService,
    private realtimeDataGateway: RealtimeTelemetryDataGateway,
    private faultService: FaultService,
  ) {}

  // This function _is_ ordered in importance
  public async addMeasurementReading(props: MeasurementReading) {
    const validatedMeasurement = this.validateMeasurementReading(props);

    if (!validatedMeasurement) {
      throw new MeasurementReadingValidationError('Invalid measurement');
    }

    const { measurement, reading } = validatedMeasurement;
    const { podId, measurementKey, value, timestamp } = reading

    // First, get the data to the client ASAP
    this.realtimeDataGateway.sendMeasurementReading({
      podId,
      measurementKey,
      value,
      timestamp
    });

    // Then check if it breaches limits
    if (measurement.format === 'float' || measurement.format === 'integer') {
      const breachLevel = doesMeasurementBreachLimits(measurement, reading);
      if (breachLevel) {
        this.logger.debug(`Measurement breached limits {${props.podId}/${props.measurementKey}}: ${breachLevel} with value ${props.value}`)
        await this.faultService.addLimitBreachFault({
          level: breachLevel,
          measurement,
          tripReading: reading
        });
      }
    }

    // Then save it to the database
    const point = new Point('measurement')
      .timestamp(timestamp)
      .tag('podId', podId)
      .tag('measurementKey', measurementKey)
      .tag('format', measurement.format)
      .floatField('value', value);

    try {
      this.influxService.telemetryWrite.writePoint(point);

      this.logger.debug(
        `Added measurement {${props.podId}/${props.measurementKey}}: ${props.value}`,
        MeasurementService.name,
      );
    } catch (e) {
      this.logger.error(
        `Failed to add measurement {${props.podId}/${props.measurementKey}}: ${props.value}`,
        e,
        MeasurementService.name,
      );
    }
  }

  private validateMeasurementReading(props: MeasurementReading) {
    const result = MeasurementReadingSchema.safeParse(props);
    if (!result.success) {
      throw new MeasurementReadingValidationError(result.error.message);
    }

    const { podId, measurementKey, value } = result.data;

    const pod = pods[podId];
    if (!pod) {
      throw new MeasurementReadingValidationError('Pod not found');
    }

    const measurement = pods[podId]['measurements'][measurementKey];
    if (!measurement) {
      throw new MeasurementReadingValidationError('Measurement not found');
    }

    if (measurement.format === 'enum') {
      const enumValue = measurement.enumerations.find((e) => e.value === value);

      if (!enumValue) {
        throw new MeasurementReadingValidationError('Invalid enum value');
      }
    }

    return {
      reading: result.data,
      measurement,
    };
  }
}
