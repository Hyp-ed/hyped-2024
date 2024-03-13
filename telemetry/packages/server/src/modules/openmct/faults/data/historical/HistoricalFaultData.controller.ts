import { Controller, Get, Param } from '@nestjs/common';
import { HistoricalFaultDataService } from './HistoricalFaultData.service';
import { HistoricalFaults } from '@hyped/telemetry-types/dist/openmct/openmct-fault.types';

@Controller('openmct/faults/historical')
export class HistoricalFaultsDataController {
  constructor(private historicalDataService: HistoricalFaultDataService) {}

  @Get()
  getAllFaults(): Promise<HistoricalFaults> {
    return this.historicalDataService.getHistoricalFaults({});
  }

  @Get('pods/:podId')
  getFaultsForPod(@Param('podId') podId: string): Promise<HistoricalFaults> {
    return this.historicalDataService.getHistoricalFaults({
      podId,
    });
  }
}
