import { Injectable, LoggerService, Inject } from '@nestjs/common';
import { MqttService } from 'nest-mqtt';
import { Logger } from '@/modules/logger/Logger.decorator';

@Injectable()
export class PodControlsService {
  constructor(
    @Inject(MqttService) private readonly mqttService: MqttService,
    @Logger()
    private readonly logger: LoggerService,
  ) {}

  /**
   * Sends a control message to a pod.
   * @param control The control message to send
   * @param podId The ID of the pod
   * @returns True if the message was sent successfully, false otherwise
   */
  async sendControlMessage(control: string, podId: string) {
    switch (control) {
      case 'start':
        // TODO: replace with state machine transition message to accelerating state
        await this.mqttService.publish(
          `hyped/${podId}/controls/${control}`,
          control,
        );
        break;
      case 'stop':
        // TODO: replace with state machine transition message to braking state
        await this.mqttService.publish(
          `hyped/${podId}/controls/${control}`,
          control,
        );
        break;
      default:
        await this.mqttService.publish(
          `hyped/${podId}/controls/${control}`,
          control,
        );
    }
    this.logger.log(
      `Control message "${control}" sent to pod "${podId}"`,
      PodControlsService.name,
    );
    return true;
  }

  /**
   * Sets the levitation height of a pod.
   * @param height The height in millimeters
   * @param podId The ID of the pod
   * @returns True if the message was sent successfully, false otherwise
   */
  async setLevitationHeight(height: number, podId: string) {
    await this.mqttService.publish(
      `hyped/${podId}/controls/levitation_height`,
      height.toString(),
    );
    this.logger.log(
      `Levitation height set to ${height}mm for pod "${podId}"`,
      PodControlsService.name,
    );
    return true;
  }
}
