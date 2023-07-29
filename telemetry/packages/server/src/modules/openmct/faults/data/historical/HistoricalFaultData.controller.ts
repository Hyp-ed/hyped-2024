import { Controller, Get, Param, Query } from '@nestjs/common';
import { HistoricalFaultDataService } from './HistoricalFaultData.service';

@Controller('openmct/faults/historical')
export class HistoricalFaultsDataController {
  constructor(private historicalDataService: HistoricalFaultDataService) {}
  @Get('pods/:podId')
  getFaults(
    @Param('podId') podId: string,
  ) {
    return this.historicalDataService.getHistoricalFaults({
      podId,
   } );
  }
}
