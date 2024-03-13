import { OpenMCT } from 'openmct/dist/openmct';
import { HistoricalFaultsProvider } from './historical-faults-provider';
import { RealtimeFaultsProvider } from './realtime-faults-provider';
import { http } from 'openmct/core/http';
import { OpenMctFault } from '@hyped/telemetry-types';
import { log } from '@/lib/logger';

/**
 * The Faults plugin for Open MCT.
 * Provides fault management for telemetry data by querying the telemetry server for faults.
 * @returns The faults plugin function.
 */
export function FaultsPlugin() {
  return function install(openmct: OpenMCT) {
    // Add the (built-in) Fault Management plugin to Open MCT
    openmct.install(openmct.plugins.FaultManagement());

    const realtimeProvider = RealtimeFaultsProvider();
    const historicalProvider = HistoricalFaultsProvider();

    // Add our fault providers to Open MCT
    openmct.faults.addProvider({
      supportsRequest: historicalProvider.supportsRequest,
      supportsSubscribe: realtimeProvider.supportsSubscribe,
      subscribe: realtimeProvider.subscribe,
      request: historicalProvider.request,
      acknowledgeFault,
    });
  };
}

/**
 * Sends an acknowledgement for a fault to the server.
 * @param fault The Open MCT fault object.
 * @param comment The comment to send with the acknowledgement.
 * In the future we could also do something with the comments, but for now we will just log them.
 */
const acknowledgeFault = async (
  fault: OpenMctFault['fault'],
  { comment }: { comment: string },
) => {
  const url = `openmct/faults/acknowledge`;
  await http.post(url, {
    body: JSON.stringify({
      faultId: fault.id,
    }),
    headers: {
      'Content-Type': 'application/json',
    },
  });
  log(
    `Acknowledged fault with id ${fault.id}.${comment ? ` Comment: ${comment}` : ''}`,
  );
};
