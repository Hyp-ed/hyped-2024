import { POD_IDS, PodId, pods } from '@hyped/telemetry-constants';
import { OpenMctDictionary, OpenMctPod } from '@hyped/telemetry-types';
import { Injectable } from '@nestjs/common';
import { mapMeasurementToOpenMct } from './utils/mapMeasurementToOpenMct';

@Injectable()
export class DictionaryService {
  getDictionary(): OpenMctDictionary {
    const dictionary: OpenMctDictionary = {};
    POD_IDS.forEach((podId) => {
      dictionary[podId] = this.getPod(podId);
    });
    return dictionary;
  }

  getPodIds() {
    return POD_IDS;
  }

  getPod(podId: string): OpenMctPod {
    this.validatePodId(podId);
    const pod = pods[podId];

    const measurements = Object.values(pod.measurements).map((measurement) =>
      mapMeasurementToOpenMct(measurement),
    );

    return {
      name: pod.name,
      id: pod.id,
      measurements,
    };
  }

  getMeasurement(podId: string, measurementKey: string) {
    this.validatePodId(podId);
    const pod = pods[podId];

    const measurement = pod.measurements[measurementKey];
    if (!measurement) {
      throw new Error(`Measurement ${measurementKey} not found`);
    }

    return mapMeasurementToOpenMct(measurement);
  }

  private validatePodId(podId: string): asserts podId is PodId {
    if (!POD_IDS.includes(podId as PodId)) {
      throw new Error(`Pod ${podId} not found`);
    }
  }
}
