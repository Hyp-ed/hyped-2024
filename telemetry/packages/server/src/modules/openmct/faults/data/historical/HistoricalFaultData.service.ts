import { INFLUX_FAULTS_BUCKET } from '@/core/config';
import { InfluxRow } from '@/modules/common/types/InfluxRow';
import { InfluxService } from '@/modules/influx/Influx.service';
import { Logger } from '@/modules/logger/Logger.decorator';
import { OpenMctFault } from '@hyped/telemetry-types';
import { fluxExpression, fluxString } from '@influxdata/influxdb-client';
import { Injectable, LoggerService } from '@nestjs/common';

interface InfluxFaultRow extends InfluxRow {
  fault: string;
}

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
      |> filter(fn: (r) => r["podId"] == ${
        fluxString(podId) as unknown as string
      })
      ${
        measurementKey
          ? `|> filter(fn: (r) => r["measurementKey"] == ${
              fluxString(measurementKey) as unknown as string
            })`
          : ''
      }
      ${
        !getAcknowledged
          ? (fluxExpression(
              `|> filter(fn: (r) => r["acknowledged"] == "false")`,
            ) as unknown as string)
          : ''
      }
      |> group(columns: ["faultId"])
      |> last()`;

    try {
      const data =
        await this.influxService.query.collectRows<InfluxFaultRow>(query);
      return data.map((row) => ({
        timestamp: new Date(row['_time']).getTime(),
        fault: JSON.parse(row['_value']) as OpenMctFault,
      }));
    } catch (e: unknown) {
      this.logger.error(
        `Failed to get faults for pod ${podId}`,
        e,
        HistoricalFaultDataService.name,
      );
    }
  }
}
