import { openMctObjectTypes } from '@hyped/constants';
import { OpenMctObjectTypes } from '@hyped/types';
import { Injectable } from '@nestjs/common';

@Injectable()
export class ObjectTypesService {
  getObjectTypes(): OpenMctObjectTypes {
    return openMctObjectTypes;
  }
}
