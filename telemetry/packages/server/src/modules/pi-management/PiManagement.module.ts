import { Module } from '@nestjs/common';
import { PiManagementController } from './PiManagement.controller';
import { PiManagementService } from './PiManagement.service';

@Module({
  controllers: [PiManagementController],
  providers: [PiManagementService],
})
export class PiManagementModule {}
