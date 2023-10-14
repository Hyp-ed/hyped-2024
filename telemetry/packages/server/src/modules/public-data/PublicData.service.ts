import { Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '../logger/Logger.decorator';
import { HistoricalTelemetryDataService } from '../openmct/data/historical/HistoricalTelemetryData.service';
import { INFLUX_TELEMETRY_BUCKET } from '../core/config';
import { flux } from '@influxdata/influxdb-client';
import { InfluxService } from '../influx/Influx.service';
import { ACTIVE_STATES } from '@hyped/telemetry-constants';

type InfluxRow = {
  _time: string;
  _value: string;
  podId: string;
  stateType: string;
};

@Injectable()
export class PublicDataService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
    private historicalTelemetryDataService: HistoricalTelemetryDataService,
    private influxService: InfluxService,
  ) {}

  public async getVelocity(
    podId: string,
    startTimestamp: string,
    endTimestamp?: string,
  ) {
    return this.historicalTelemetryDataService.getHistoricalReading(
      podId,
      'velocity',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }

  public async getDisplacement(
    podId: string,
    startTimestamp: string,
    endTimestamp?: string,
  ) {
    return this.historicalTelemetryDataService.getHistoricalReading(
      podId,
      'displacement',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }

  public async getLevitationHeight(
    podId: string,
    startTimestamp: string,
    endTimestamp?: string,
  ) {
    return this.historicalTelemetryDataService.getHistoricalReading(
      podId,
      'levitation_height',
      startTimestamp,
      endTimestamp ?? new Date().getTime().toString(),
    );
  }

  public async getState(podId: string) {
    // Get the last state reading (measurement name should be 'state')
    const query = flux`
      from(bucket: "${INFLUX_TELEMETRY_BUCKET}")
        |> range(start: -1d)
        |> filter(fn: (r) => r["_measurement"] == "state")
        |> filter(fn: (r) => r["podId"] == "${podId}")
        |> group()
        |> sort(columns: ["_time"], desc: true)
        |> limit(n: 2)
    `;

    try {
      const data = await this.influxService.query.collectRows<InfluxRow>(query);

      return {
        currentState: {
          state: data[0]['_value'],
          timestamp: new Date(data[0]['_time']).getTime(),
          stateType: data[0]['stateType'],
        },
        previousState: {
          state: data[1]['_value'],
          timestamp: new Date(data[1]['_time']).getTime(),
          stateType: data[1]['stateType'],
        },
      };
    } catch (e) {
      this.logger.error(
        `Failed to get historical reading for ${podId}'s state`,
        e,
        PublicDataService.name,
      );
    }
  }

  // We defined "launched" as when the pod's state changes from "READY" to "ACCELERATING", and we must currently be in active state.
  public async getLaunchTime(podId: string) {
    const currentState = await this.getState(podId);
    if (
      !Object.keys(ACTIVE_STATES).includes(
        currentState?.currentState.state as string,
      )
    ) {
      return {
        launchTime: null,
      };
    }

    const query = flux`
      from(bucket: "${INFLUX_TELEMETRY_BUCKET}")
        |> range(start: -1d)
        |> filter(fn: (r) => r["_measurement"] == "state")
        |> filter(fn: (r) => r["podId"] == "${podId}")
        |> filter(fn: (r) => r["_value"] == "ACCELERATING")
        |> group()
        |> sort(columns: ["_time"], desc: true)
        |> limit(n: 1)
    `;

    try {
      const data = await this.influxService.query.collectRows<InfluxRow>(query);

      return {
        launchTime: new Date(data[0]['_time']).getTime(),
      };
    } catch (e) {
      this.logger.error(
        `Failed to get launch time for ${podId}`,
        e,
        PublicDataService.name,
      );
    }
  }
}
