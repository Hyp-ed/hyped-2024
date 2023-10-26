import { LayoutDashboard, Terminal } from 'lucide-react';
import { useState } from 'react';
import { cn } from '@/lib/utils';
import { LogViewer } from './log-viewer';
import { OpenMCT } from './openmct-iframe';

// TODO: could add state machine diagram here
const COMPONENTS = [
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
];

export default function Nav() {
  const [selectedComponent, setSelectedComponent] = useState(0);

  return (
    <div className="col-span-8 bg-hyped-background flex">
      <div
        className={cn(
          'h-[100vh] min-w-max border-r-[0px] border-r-openmct-light-gray ',
        )}
      >
        {/* <button onClick={() => setShowSidebar(!showSidebar)}>
          <Menu width={18} />
        </button> */}
        <div className="flex flex-col justify-start gap-2 py-8 px-4">
          {COMPONENTS.map((component, index) => (
            <button
              className={cn(
                'flex items-start justify-start rounded-md px-3 py-2 gap-2',
                index === selectedComponent ? 'bg-openmct-dark-gray' : '',
                'hover:bg-openmct-dark-gray transition',
              )}
              onClick={() => setSelectedComponent(index)}
            >
              {component.icon} {component.name}
            </button>
          ))}
        </div>
      </div>
      <div className="h-full w-full py-4">
        {COMPONENTS.map((component) => (
          <div
            className={cn(
              COMPONENTS.indexOf(component) === selectedComponent
                ? 'h-full border-[2px] border-openmct-dark-gray rounded-xl p-2'
                : 'hidden',
            )}
          >
            {component.component}
          </div>
        ))}
      </div>
    </div>
  );
}
