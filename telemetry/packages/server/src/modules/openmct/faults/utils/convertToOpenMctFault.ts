import { OpenMctFault } from '@hyped/telemetry-types';
import { nanoid } from 'nanoid';
import { Fault } from '../Fault.service';

export function convertToOpenMctFault(
  fault: Fault
): OpenMctFault {
  const { measurement, tripReading, level } = fault;

  const namespace = `/${tripReading.podId}/${measurement.key}`;
  return {
    type: 'global-alarm-status',
    fault: {
      id: `${namespace}-${nanoid()}`,
      name: `${measurement.name} is out of range`,
      namespace,
      seqNum: 0,
      severity: level,
      shortDescription: '',
      shelved: false,
      acknowledged: false,
      triggerTime: tripReading.timestamp.toString(),
      triggerValueInfo: {
        value: tripReading.value,
        rangeCondition: level,
        monitoringResult: level,
      },
      currentValueInfo: {
        value: tripReading.value,
        rangeCondition: level,
        monitoringResult: level,
      },
    },
  };
}
