import { Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '../logger/Logger.decorator';
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
    private influxService: InfluxService,
  ) {}

  /**
   * Get the current state of a pod.
   * @param podId The pod's ID.
   * @returns The current state and the previous state of the pod.
   */
  public async getState(podId: string) {
    // Get the last state reading from InfluxDB (measurement name should be 'state')
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

  /**
   * Get the launch time of a pod.
   * We consider the launch time to be when the pod's state changes from "READY" to "ACCELERATING", and we must currently be in active state or "STOPPED".
   * @param podId The pod's ID.
   * @returns The launch time of the pod.
   */
  public async getLaunchTime(podId: string) {
    const currentState = await this.getState(podId);

    // If the pod is not in an active state or stopped, launch time isn't defined
    if (
      !(
        Object.keys(ACTIVE_STATES).includes(
          currentState?.currentState.state as string,
        ) || currentState?.currentState.state === 'STOPPED'
      )
    ) {
      return {
        launchTime: null,
      };
    }

    // Get the last "ACCELERATING" state reading from InfluxDB
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
      const launchTime = new Date(data[0]['_time']).getTime();

      return {
        launchTime,
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
