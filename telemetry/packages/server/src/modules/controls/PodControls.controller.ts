import { Body, Controller, Param, Post } from '@nestjs/common';
import { PodControlsService } from './PodControls.service';

@Controller('pods/:podId/controls')
export class PodControlsController {
  constructor(private podControlsService: PodControlsService) {}

  @Post('start')
  startPod(@Param('podId') podId: string, @Body() options: any) {
    return this.podControlsService.startPod(podId, options);
  }

  @Post(':control')
  controlPod(@Param('control') control: string, @Param('podId') podId: string) {
    return this.podControlsService.sendControlMessage(control, podId);
  }
}
