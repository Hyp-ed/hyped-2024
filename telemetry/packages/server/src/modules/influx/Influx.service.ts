import { InfluxDB, QueryApi, WriteApi } from '@influxdata/influxdb-client';
import { Injectable, OnModuleInit } from '@nestjs/common';
import {
  INFLUX_TELEMETRY_BUCKET,
  INFLUX_ORG,
  INFLUX_TOKEN,
  INFLUX_URL,
  INFLUX_FAULTS_BUCKET,
} from '../core/config';

@Injectable()
export class InfluxService implements OnModuleInit {
  private connection: InfluxDB;
  public telemetryWrite: WriteApi;
  public faultsWrite: WriteApi;
  public query: QueryApi;

  async $connect() {
    this.connection = new InfluxDB({ url: INFLUX_URL, token: INFLUX_TOKEN });
    this.telemetryWrite = this.connection.getWriteApi(INFLUX_ORG, INFLUX_TELEMETRY_BUCKET, 'ns', {
      batchSize: 1,
    });
    this.faultsWrite = this.connection.getWriteApi(INFLUX_ORG, INFLUX_FAULTS_BUCKET, 'ns', {
      batchSize: 1,
    });
    this.query = this.connection.getQueryApi(INFLUX_ORG);
  }

  async onModuleInit() {
    await this.$connect();
    if (!this.telemetryWrite) {
      throw new Error('InfluxDB telemetry write API not initialized');
    }

    if (!this.faultsWrite) {
      throw new Error('InfluxDB faults write API not initialized');
    }

    if (!this.query) {
      throw new Error('InfluxDB query API not initialized');
    }
  }

  async onModuleDestroy() {
    try {
      await this.telemetryWrite.close();
      await this.faultsWrite.close();
    } catch (e) {
      console.error(e);
    }
  }
}
