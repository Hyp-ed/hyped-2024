import { Injectable, LoggerService, Inject } from '@nestjs/common';
import { MqttService } from 'nest-mqtt';
import { Logger } from '../logger/Logger.decorator';

@Injectable()
export class PodControlsService {
  constructor(
    @Inject(MqttService) private readonly mqttService: MqttService,
    @Logger()
    private readonly logger: LoggerService,
  ) {}

  // Sends a control message to the pod over MQTT
  async sendControlMessage(control: string, podId: string) {
    this.mqttService.publish(`hyped/${podId}/controls/${control}`, control);
    this.logger.log(
      `Control message "${control}" sent to pod "${podId}"`,
      PodControlsService.name,
    );
    return true;
  }

  // Sends a start message to the pod over MQTT, with options
  async startPod(podId: string, options: any) {
    this.mqttService.publish(`hyped/${podId}/controls/start`, options);
    this.logger.log(
      `Control message "start" sent to pod "${podId}"`,
      PodControlsService.name,
    );
    return true;
  }
}
