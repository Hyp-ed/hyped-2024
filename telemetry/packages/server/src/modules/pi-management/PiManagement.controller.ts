import {
  Controller,
  Get,
  HttpException,
  Param,
  Post,
  Query,
} from '@nestjs/common';
import { PiManagementService } from './PiManagement.service';
import { validatePodId } from '../common/utils/validatePodId';
import { PodId, pods } from '@hyped/telemetry-constants';

@Controller('pods/:podId/pis')
export class PiManagementController {
  constructor(private piManagementService: PiManagementService) {}

  @Get()
  async getAllPis(
    @Param('podId') podId: string,
    @Query('compareBranch') compareBranch: string,
  ) {
    validatePodId(podId);
    return this.piManagementService.getAllPis(podId, compareBranch);
  }

  @Get(':piId')
  async getPi(
    @Param('podId') podId: string,
    @Param('piId') piId: string,
    @Query('compareBranch') compareBranch: string,
  ) {
    validatePodId(podId);
    this.validatePiId(podId, piId);
    return this.piManagementService.getPi(podId, piId, compareBranch);
  }

  @Post(':piId/update-binary')
  async updatePiBinary(
    @Param('podId') podId: string,
    @Param('piId') piId: string,
    @Query('compareBranch') compareBranch: string,
  ) {
    validatePodId(podId);
    this.validatePiId(podId, piId);
    return this.piManagementService.updatePiBinary(podId, piId, compareBranch);
  }

  @Post(':piId/update-config')
  async updatePiConfig(
    @Param('podId') podId: string,
    @Param('piId') piId: string,
    @Query('compareBranch') compareBranch: string,
  ) {
    validatePodId(podId);
    this.validatePiId(podId, piId);
    return this.piManagementService.updatePiConfig(podId, piId, compareBranch);
  }

  private validatePiId(podId: PodId, piId: string) {
    const pi = pods[podId].pis[piId];
    if (!pi) {
      throw new HttpException(`Unknown pi ID: ${piId} on pod ${podId}`, 400);
    }
    return pi;
  }
}
