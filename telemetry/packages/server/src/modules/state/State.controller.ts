import { Body, Controller, Param, Post } from '@nestjs/common';
import { StateService } from './State.service';

@Controller('pods/:podId/state')
export class StateController {
  constructor(private stateService: StateService) {}

  @Post('set')
  setPodState(@Param('podId') podId: string, @Body() body: any) {
    return this.stateService.setPodState(podId, body.state);
  }
}
