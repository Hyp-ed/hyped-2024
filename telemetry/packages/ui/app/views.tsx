import { Bug, GitBranch, LineChart, Terminal } from 'lucide-react';
import { LogViewer } from './components/log-viewer';
import { OpenMCT } from './components/openmct-iframe';
import { DebugView } from './components/debug-view/debug-view';
import { StateMachine } from './components/flow/state-machine';

/**
 * The components that can be rendered in the LHS of the GUI.
 */
export const VIEWS = [
  {
    name: 'OpenMCT',
    component: <OpenMCT />,
    icon: <LineChart width={18} />,
  },
  {
    name: 'Logs',
    component: <LogViewer />,
    icon: <Terminal width={18} />,
  },
  {
    name: 'Debug View',
    component: <DebugView />,
    icon: <Bug width={18} />,
  },
  {
    name: 'State',
    component: <StateMachine />,
    icon: <GitBranch width={18} />,
  },
];
