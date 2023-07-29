import { Injectable, LoggerService } from '@nestjs/common';
import { Logger } from '../logger/Logger.decorator';

@Injectable()
export class RemoteLogsService {
  constructor(
    @Logger()
    private readonly logger: LoggerService,
  ) {}

  async logRemoteMessage(message: string) {
    this.logger.log(`Pod log from UI: ${message}`, RemoteLogsService.name);
    return true;
  }

  async logRemoteMessageWithPodID(podId: string, message: string) {
    this.logger.log(
      `Pod "${podId}" log from UI: ${message}`,
      RemoteLogsService.name,
    );
    return true;
  }
}
