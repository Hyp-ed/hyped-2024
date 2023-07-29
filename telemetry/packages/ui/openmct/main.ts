import openmct from 'openmct/dist/openmct';

import { DictionaryPlugin } from './plugins/dictionary-plugin';
import { HistoricalTelemetryPlugin } from './plugins/historical-telemetry-plugin';
import { RealtimeTelemetryPlugin } from './plugins/realtime-telemetry-plugin';
import { LimitPlugin } from './plugins/limit-plugin';
import { ConductorPlugin } from './plugins/conductor';
import { FaultsPlugin } from './plugins/faults-plugin/faults-plugin';

const TEN_SECONDS = 10 * 1000;

openmct.setAssetPath('/openmct-lib');

// Local storage of dashbaords
openmct.install(openmct.plugins.LocalStorage());

// Time
openmct.install(openmct.plugins.UTCTimeSystem());
openmct.time.clock('local', { start: -TEN_SECONDS, end: 0 });

// Theme
openmct.install(openmct.plugins['Espresso']());

// Views
openmct.install(openmct.plugins.MyItems());
openmct.install(openmct.plugins.PlanLayout());
openmct.install(openmct.plugins.Timeline());
openmct.install(openmct.plugins.Hyperlink());
openmct.install(
  openmct.plugins.AutoflowView({
    type: 'telemetry.panel',
  }),
);
openmct.install(
  openmct.plugins.DisplayLayout({
    showAsView: ['summary-widget', 'example.imagery'],
  }),
);
openmct.install(openmct.plugins.LADTable());
openmct.install(openmct.plugins.SummaryWidget());
openmct.install(openmct.plugins.Timer());
openmct.install(openmct.plugins.Timelist());
openmct.install(openmct.plugins.BarChart());
openmct.install(openmct.plugins.ScatterPlot());
openmct.install(ConductorPlugin())

// Data
openmct.install(DictionaryPlugin());
openmct.install(HistoricalTelemetryPlugin());
openmct.install(RealtimeTelemetryPlugin());

// Limits/faults
openmct.install(LimitPlugin());
openmct.install(FaultsPlugin());

// Dashboards
// openmct.install(openmct.plugins.StaticRootPlugin('dashboards', 'data/dashboards.json'));

function sleep(ms: number) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

// Wait for all plugins to be installed before starting
sleep(1000).then(() => {
  openmct.start();
});
