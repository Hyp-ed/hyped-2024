import { Cpu, GitBranch, LayoutDashboard, Terminal } from 'lucide-react';
import { StateMachineFlowChart } from './components/flow/flow-chart';
import { LogViewer } from './components/log-viewer';
import { OpenMCT } from './components/openmct-iframe';
import { PiManagement } from './components/pi-management/pi-management';

/**
 * The components that can be rendered in the LHS of the GUI.
 */
export const VIEWS = [
  {
    name: 'OpenMCT',
    component: <OpenMCT />,
    icon: <LayoutDashboard width={18} />,
  },
  {
    name: 'Logs',
    component: <LogViewer />,
    icon: <Terminal width={18} />,
  },
  {
    name: 'State',
    component: <StateMachineFlowChart currentState="ACCELERATING" />,
    icon: <GitBranch width={18} />,
  },
  {
    name: 'Pi Management',
    component: <PiManagement />,
    icon: <Cpu width={18} />,
  },
];
