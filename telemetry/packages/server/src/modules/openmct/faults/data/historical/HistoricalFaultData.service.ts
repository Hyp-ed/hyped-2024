import { INFLUX_FAULTS_BUCKET } from '@/core/config';
import { InfluxService } from '@/modules/influx/Influx.service';
import { Logger } from '@/modules/logger/Logger.decorator';
import { OpenMctFault } from '@hyped/telemetry-types';
import {
  flux,
  fluxBool,
  fluxExpression,
  fluxString,
} from '@influxdata/influxdb-client';
import { Injectable, LoggerService } from '@nestjs/common';

type InfluxRow = {
  _time: string;
  _value: string;
  podId: string;
  fault: string;
};

type GetHistoricalFaultsInput = {
  podId: string;
  measurementKey?: string;
};

type GetHistoricalFaultsOptions = {
  includeAcknowledged?: boolean;
};

export type GetHistoricalFaultsReturn = {
  timestamp: number;
  fault: OpenMctFault;
}[];

@Injectable()
export class HistoricalFaultDataService {
  constructor(
    private influxService: InfluxService,
    @Logger()
    private readonly logger: LoggerService,
  ) {}

  public async getHistoricalFaults(
    props: GetHistoricalFaultsInput,
    options: GetHistoricalFaultsOptions = {
      includeAcknowledged: true,
    },
  ) {
    const { podId, measurementKey } = props;
    const { includeAcknowledged: getAcknowledged } = options;

    const query = `from(bucket: "${INFLUX_FAULTS_BUCKET}")
      |> range(start: -24h)
      |> filter(fn: (r) => r["podId"] == ${fluxString(podId)})
      ${
        measurementKey
          ? `|> filter(fn: (r) => r["measurementKey"] == ${fluxString(
              measurementKey,
            )})`
          : ''
      }
      ${
        !getAcknowledged
          ? fluxExpression(`|> filter(fn: (r) => r["acknowledged"] == "false")`)
          : ''
      }
      |> group(columns: ["faultId"])
      |> last()`;

    try {
      const data = await this.influxService.query.collectRows<InfluxRow>(query);
      return data.map((row) => ({
        timestamp: new Date(row['_time']).getTime(),
        fault: JSON.parse(row['_value']) as OpenMctFault,
      }));
    } catch (e) {
      this.logger.error(
        `Failed to get faults for pod ${podId}`,
        e,
        HistoricalFaultDataService.name,
      );
    }
  }
}
