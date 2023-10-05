import { OpenMCT } from 'openmct/dist/openmct';
import { HistoricalFaultsProvider } from './historical-faults-provider';
import { RealtimeFaultsProvider } from './realtime-faults-provider';

export function FaultsPlugin() {
  return function install(openmct: OpenMCT) {
    openmct.install(openmct.plugins.FaultManagement());

    const realtimeProvider = RealtimeFaultsProvider();
    const historicalProvider = HistoricalFaultsProvider();

    openmct.faults.addProvider({
      supportsRequest: historicalProvider.supportsRequest,
      supportsSubscribe: realtimeProvider.supportsSubscribe,
      subscribe: realtimeProvider.subscribe,
      request: historicalProvider.request,
    });
  };
}
