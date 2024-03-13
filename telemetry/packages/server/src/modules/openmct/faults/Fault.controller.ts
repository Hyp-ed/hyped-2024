import { Body, Controller, Post } from '@nestjs/common';
import { FaultService } from './Fault.service';

@Controller('openmct/faults')
export class FaultsController {
  constructor(private faultsService: FaultService) {}

  @Post('acknowledge')
  acknowledgeFault(
    @Body() { faultId, comment }: { faultId: string; comment: string },
  ) {
    return this.faultsService.acknowledgeFault(faultId, comment);
  }

  // @Get('shelve/:faultId')
  // shelveFault(@Param('faultId') faultId: string) {
  //   return this.faultsService.shelveFault(faultId);
  // }
}
