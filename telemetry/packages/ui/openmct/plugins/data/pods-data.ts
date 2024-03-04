import { OpenMctMeasurement, OpenMctPod } from '@hyped/telemetry-types';
import { http } from '../../core/http';

export async function fetchPodIds() {
  return http
    .get('openmct/dictionary/pods')
    .json<{ ids: string[] }>()
    .then((data) => {
      return data;
    });
}

export async function fetchPod(id: string) {
  return http
    .get(`openmct/dictionary/pods/${id}`)
    .json<OpenMctPod>()
    .then((data) => {
      return data;
    });
}

export async function fetchMeasurement(podId: string, measurementKey: string) {
  return http
    .get(`openmct/dictionary/pods/${podId}/measurements/${measurementKey}`)
    .json<OpenMctMeasurement>()
    .then((data) => {
      return data;
    });
}
