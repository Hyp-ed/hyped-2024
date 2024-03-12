import { OpenMCT } from 'openmct/dist/openmct';
import { HistoricalFaultsProvider } from './historical-faults-provider';
import { RealtimeFaultsProvider } from './realtime-faults-provider';

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
    });
  };
}
