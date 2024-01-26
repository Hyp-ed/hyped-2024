import { Controller, Get, HttpException, Param } from '@nestjs/common';
import { PiManagementService } from './PiManagement.service';
import { validatePodId } from '../common/utils/validatePodId';
import { pods } from '@hyped/telemetry-constants';

@Controller('pods/:podId/pis')
export class PiManagementController {
  constructor(private piManagementService: PiManagementService) {}

  @Get()
  async getAllPis(@Param('podId') podId: string) {
    validatePodId(podId);
    return this.piManagementService.getAllPis(podId);
  }

  @Get(':piId')
  async getPi(@Param('podId') podId: string, @Param('piId') piId: string) {
    validatePodId(podId);
    this.validatePiId(podId, piId);
    return this.piManagementService.getPi(podId, piId);
  }

  @Get(':piId/update-code')
  async updatePiCode(
    @Param('podId') podId: string,
    @Param('piId') piId: string,
  ) {
    validatePodId(podId);
    this.validatePiId(podId, piId);
    return this.piManagementService.updatePiCode(podId, piId);
  }

  @Get(':piId/update-config')
  async updatePiConfig(
    @Param('podId') podId: string,
    @Param('piId') piId: string,
  ) {
    validatePodId(podId);
    this.validatePiId(podId, piId);
    return this.piManagementService.updatePiConfig(podId, piId);
  }

  private validatePiId(podId: string, piId: string) {
    const pi = pods[podId].pis[piId];
    if (!pi) {
      throw new HttpException(`Unknown pi ID: ${piId} on pod ${podId}`, 400);
    }
    return pi;
  }
}
