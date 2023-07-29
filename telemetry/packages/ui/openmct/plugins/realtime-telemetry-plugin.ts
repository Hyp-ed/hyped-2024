import { OpenMCT } from 'openmct/dist/openmct';
import { io } from 'socket.io-client';
import { SERVER_ENDPOINT } from '../core/config';
import { AugmentedDomainObject } from '../types/AugmentedDomainObject';
import { socket as socketConstants } from '@hyped/telemetry-constants'

export function RealtimeTelemetryPlugin() {
  return function install(openmct: OpenMCT) {
    const socket = io(SERVER_ENDPOINT, { path: '/openmct/data/realtime' });
    // handle socket disconnects

    const listenerCallbacks: any = {};

    socket.on(socketConstants.MEASUREMENT_EVENT, (data) => {
      const { podId, measurementKey, value, timestamp } = data;
      const roomName = socketConstants.getMeasurementRoomName(podId, measurementKey);

      if (listenerCallbacks[roomName]) {
        listenerCallbacks[roomName]({ id: measurementKey, value, timestamp });
      }
    });

    const provider = {
      supportsSubscribe: (domainObject: AugmentedDomainObject) => {
        return domainObject.podId !== undefined;
      },
      subscribe: (domainObject: AugmentedDomainObject, callback: any) => {
        const { podId, identifier } = domainObject;
        const roomName = socketConstants.getMeasurementRoomName(podId, identifier.key);

        listenerCallbacks[roomName] = callback;
        socket.emit(socketConstants.EVENTS.SUBSCRIBE_TO_MEASUREMENT, roomName);

        return function unsubscribe() {
          delete listenerCallbacks[roomName];
          socket.emit(socketConstants.EVENTS.UNSUBSCRIBE_FROM_MEASUREMENT, roomName);
        };
      },
    };

    openmct.telemetry.addProvider(provider);
  };
}
