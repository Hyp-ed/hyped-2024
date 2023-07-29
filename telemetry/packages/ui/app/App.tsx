import { Logo, ConnectionStatus } from './components';
import { PodControls } from './components/pod-controls';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from './components/ui/select';
import { useState } from 'react';
import { PodDisconnectError } from './components/pod-disconnect-error';
import { useMQTT } from './context/mqtt';
import { Latency } from './components/latency';
import { POD_IDS } from '@hyped/telemetry-constants';
import { usePod } from './context/pods';

const DEFAULT_POD = POD_IDS[0];

const App = () => {
  const [currentPod, setCurrentPod] = useState<string>(DEFAULT_POD);
  const { connectionStatus } = usePod(currentPod);

  return (
    <main className="px-4 py-8 flex flex-col gap-2 justify-between h-full bg-[#393939] select-none text-gray-100">
      <div className="flex flex-col justify-between h-full">
        {/* Status, Latency, State, Title */}
        <div className="flex flex-col gap-2">
          <div className="flex flex-col gap-1">
            <ConnectionStatus podId={currentPod} />
            <PodDisconnectError status={connectionStatus} podId={currentPod} />
            <Latency podId={currentPod} />
          </div>
          <h1 className="text-5xl font-title font-black my-2">Controls</h1>
        </div>
        <div className="flex flex-col justify-start h-full mt-4">
          {/* Select component to decide which pod to show the controls for */}
          <Select
            onValueChange={(podId) => setCurrentPod(podId)}
            defaultValue={currentPod}
            style={{ width: 'full' }}
          >
            <SelectTrigger>
              <SelectValue placeholder="Pod" />
            </SelectTrigger>
            <SelectContent>
              {POD_IDS.map((podId) => (
                <SelectItem key={podId} value={podId}>
                  {podId}
                </SelectItem>
              ))}
            </SelectContent>
          </Select>
          {POD_IDS.map((podId) => (
            <PodControls
              key={podId}
              podId={podId}
              show={currentPod === podId}
            />
          ))}
        </div>
        <Logo />
      </div>
    </main>
  );
};

export default App;
