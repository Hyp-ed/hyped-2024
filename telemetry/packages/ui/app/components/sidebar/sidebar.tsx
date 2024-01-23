import { VIEWS } from '@/views';
import { log } from '@/lib/logger';
import { cn } from '@/lib/utils';
import { POD_IDS } from '@hyped/telemetry-constants';
import { useEffect } from 'react';
import toast from 'react-hot-toast';
import { useCurrentPod } from '../../context/pods';
import { Latency } from './latency';
import { PodControls } from './pod-controls';
import { PodDisconnectError } from '../pod-disconnect-error';
import { PodConnectionStatus } from './pod-connection-status';
import { Logo } from '../shared/logo';
import { PodSelector } from './pod-selector';

/**
 * The custom sidebar for the GUI which allows us to select a pod, control it, view its connection status, and change the view.
 * Formerly known as the "Controls UI"
 */
export const Sidebar = ({
  selectedComponent,
  setSelectedComponent,
}: {
  selectedComponent: number;
  setSelectedComponent: React.Dispatch<React.SetStateAction<number>>;
}) => {
  const {
    currentPod,
    pod: { podState, connectionStatus },
  } = useCurrentPod();

  // Display notification when the pod state changes
  useEffect(() => {
    toast(`Pod state changed: ${podState}`);
    log(`Pod state changed: ${podState}`, currentPod);
  }, [podState]);

  return (
    <main className="col-span-1 h-[100vh] border-l-[0px] border-l-openmct-light-gray px-4 py-8 flex flex-col justify-between bg-hyped-background select-none text-gray-100">
      <div className="flex flex-col gap-12 h-full">
        <PodSelector />
        {/* Status, Latency, State, Title */}
        <div className="flex flex-col gap-2">
          <p className="font-bold text-xl">Connection to pod</p>
          <PodConnectionStatus podId={currentPod} />
          <PodDisconnectError status={connectionStatus} podId={currentPod} />
          <Latency podId={currentPod} />
        </div>
        <div className="flex flex-col justify-start">
          <p className="font-bold text-xl">Controls</p>
          {POD_IDS.map((podId) => (
            <PodControls
              key={podId}
              podId={podId}
              show={currentPod === podId}
            />
          ))}
        </div>
        <div>
          <p className="font-bold text-xl">View</p>
          <div className="h-full py-2 flex flex-col justify-start gap-2">
            {VIEWS.map((component, index) => (
              <button
                className={cn(
                  'flex items-start justify-start rounded-md px-3 py-2 gap-2',
                  index === selectedComponent ? 'bg-openmct-dark-gray' : '',
                  'hover:ring-1 hover:ring-openmct-light-gray transition',
                )}
                onClick={() => setSelectedComponent(index)}
              >
                {component.icon} {component.name}
              </button>
            ))}
          </div>
        </div>
      </div>
      <Logo />
    </main>
  );
};
