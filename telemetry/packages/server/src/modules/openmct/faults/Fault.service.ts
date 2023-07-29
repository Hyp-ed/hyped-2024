import { InfluxService } from '@/modules/influx/Influx.service';
import { Logger } from '@/modules/logger/Logger.decorator';
import { MeasurementReading } from '@/modules/measurement/MeasurementReading.types';
import { FaultLevel } from '@hyped/telemetry-constants';
import { OpenMctFault, Unpacked } from '@hyped/telemetry-types';
import { RangeMeasurement } from '@hyped/telemetry-types/dist/pods/pods.types';
import { Point } from '@influxdata/influxdb-client';
import { Injectable, LoggerService } from '@nestjs/common';
import {
  GetHistoricalFaultsReturn,
  HistoricalFaultDataService,
} from './data/historical/HistoricalFaultData.service';
import { RealtimeFaultDataGateway } from './data/realtime/RealtimeFaultData.gateway';
import { convertToOpenMctFault } from './utils/convertToOpenMctFault';
import { INFLUX_FAULTS_BUCKET, INFLUX_ORG } from '@/modules/core/config';

export type Fault = {
  level: FaultLevel;
  measurement: RangeMeasurement;
  tripReading: MeasurementReading;
};

@Injectable()
export class FaultService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
    private influxService: InfluxService,
    private historicalService: HistoricalFaultDataService,
    private realtimeService: RealtimeFaultDataGateway,
  ) {}

  public async addLimitBreachFault(fault: Fault) {
    const { measurement, tripReading } = fault;

    const possibleExistingFaults =
      await this.historicalService.getHistoricalFaults(
        {
          podId: tripReading.podId,
          measurementKey: measurement.key,
        },
        { includeAcknowledged: false },
      );

    if (possibleExistingFaults && possibleExistingFaults?.length > 0) {
      const existingFault = possibleExistingFaults[0];
      this.logger.debug(
        `Found existing fault ${existingFault.fault.fault.id}, updating`,
        FaultService.name,
      );
      await this.updateExistingFault(existingFault, tripReading);
      return;
    }

    const openMctFault = convertToOpenMctFault(fault);
    await this.saveFault(fault, openMctFault);
    this.realtimeService.sendFault(openMctFault);
  }

  private async saveFault(fault: Fault, openMctFault: OpenMctFault) {
    const { measurement, tripReading } = fault;

    const point = new Point('fault')
      .timestamp(tripReading.timestamp)
      .tag('faultId', openMctFault.fault.id)
      .tag('podId', tripReading.podId)
      .tag('measurementKey', measurement.key)
      .tag('acknowledged', 'false')
      // is influx the right choice? probably not - but we're already using it for telemetry
      .stringField('fault', JSON.stringify(openMctFault));

    try {
      this.influxService.faultsWrite.writePoint(point);

      this.logger.debug(
        `Adding fault with id ${openMctFault.fault.id}`,
        FaultService.name,
      );
    } catch (e) {
      this.logger.error(
        `Failed to add fault {${openMctFault.fault.id}}`,
        e,
        FaultService.name,
      );
    }
  }

  private async updateExistingFault(
    influxFault: Unpacked<GetHistoricalFaultsReturn>,
    updatedReading: MeasurementReading,
  ) {
    const updatedFault = influxFault.fault;
    updatedFault.fault.currentValueInfo.value = updatedReading.value;

    const point = new Point('fault')
      .timestamp(updatedReading.timestamp)
      .tag('faultId', updatedFault.fault.id)
      .tag('podId', updatedReading.podId)
      .tag('measurementKey', updatedReading.measurementKey)
      .tag('acknowledged', influxFault.fault.fault.acknowledged.toString())
      .stringField('fault', JSON.stringify(updatedFault));

    try {
      this.influxService.faultsWrite.writePoint(point);

      this.logger.debug(
        `Updating fault with id ${updatedFault.fault.id}`,
        FaultService.name,
      );
    } catch (e) {
      this.logger.error(
        `Failed to update fault with id ${updatedFault.fault.id}`,
        e,
        FaultService.name,
      );
    }

    this.realtimeService.sendFault(updatedFault);
  }
}
