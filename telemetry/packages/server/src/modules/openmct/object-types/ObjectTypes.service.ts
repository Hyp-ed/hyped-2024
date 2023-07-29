import { openMctObjectTypes } from '@hyped/telemetry-constants';
import { OpenMctObjectTypes } from '@hyped/telemetry-types';
import { Injectable } from '@nestjs/common';

@Injectable()
export class ObjectTypesService {
  getObjectTypes(): OpenMctObjectTypes {
    return openMctObjectTypes;
  }
}
