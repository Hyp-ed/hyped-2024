import { Body, Controller, Param, Post } from '@nestjs/common';
import { RemoteLogsService } from './RemoteLogs.service';

@Controller('logs')
export class RemoteLogsController {
  constructor(private remoteLogsService: RemoteLogsService) {}

  @Post()
  logUIMessage(@Body() body: any) {
    return this.remoteLogsService.logRemoteMessage(body.message);
  }

  @Post(':podId')
  logUIMessageWithPodID(@Param('podId') podId: string, @Body() body: any) {
    return this.remoteLogsService.logRemoteMessageWithPodID(
      podId,
      body.message,
    );
  }
}
