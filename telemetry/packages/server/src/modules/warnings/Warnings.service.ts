import { Injectable, LoggerService, Inject } from '@nestjs/common';
import { MqttService } from 'nest-mqtt';
import { Logger } from '../logger/Logger.decorator';

@Injectable()
export class WarningsService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
  ) {}

  async createLatencyWarning(podId: string) {
    this.logger.log(
      `Creating latency warning for pod ${podId}`,
      WarningsService.name,
    );
  }
}
