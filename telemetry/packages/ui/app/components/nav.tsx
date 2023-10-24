import { LayoutDashboard, Terminal } from 'lucide-react';
import { useState } from 'react';
import { cn } from '@/lib/utils';
import { LogViewer } from './log-viewer';
import { OpenMCT } from './openmct-iframe';

// TODO: could add state machine here
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
  const [showSidebar, setShowSidebar] = useState(false);
  const [selectedComponent, setSelectedComponent] = useState(0);

  return (
    <div className="col-span-5 bg-black flex">
      <div
        className={cn('h-[100vh] min-w-max border-r-[0px] border-r-[#535353] ')}
      >
        {/* <button onClick={() => setShowSidebar(!showSidebar)}>
          <Menu width={18} />
        </button> */}
        <div className="flex flex-col justify-start gap-2 mt-4 pl-2">
          {COMPONENTS.map((component, index) => (
            <button
              className={cn(
                'flex items-start justify-start rounded-md px-3 py-2 gap-2',
                index === selectedComponent ? 'bg-[#222222]' : '',
                'hover:bg-[#222222] transition',
              )}
              onClick={() => setSelectedComponent(index)}
            >
              {component.icon} {component.name}
            </button>
          ))}
        </div>
      </div>
      <div className="h-full w-full p-4">
        {COMPONENTS.map((component) => (
          <div
            className={cn(
              COMPONENTS.indexOf(component) === selectedComponent
                ? 'h-full border-[2px] border-[#222222] rounded-xl p-2'
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
