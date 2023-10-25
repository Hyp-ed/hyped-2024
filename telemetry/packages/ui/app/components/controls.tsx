import { Logo, ConnectionStatus, PodState } from '.';
import { PodControls } from './pod-controls';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from './ui/select';
import { useEffect, useState } from 'react';
import { PodDisconnectError } from './pod-disconnect-error';
import { Latency } from './latency';
import { POD_IDS } from '@hyped/telemetry-constants';
import { usePod } from '../context/pods';
import toast from 'react-hot-toast';
import { log } from '@/lib/logger';

const DEFAULT_POD = POD_IDS[0];

export const ControlsUI = () => {
  const [currentPod, setCurrentPod] = useState<string>(DEFAULT_POD);
  const { connectionStatus } = usePod(currentPod);
  const { podState } = usePod(currentPod);

  // Display notification when the pod state changes
  useEffect(() => {
    toast(`Pod state changed: ${podState}`);
    log(`Pod state changed: ${podState}`, currentPod);
  }, [podState]);

  return (
    <main className="col-span-1 h-[100vh] border-l-[0px] border-l-[#535353] px-4 py-8 flex flex-col gap-2 justify-between bg-black select-none text-gray-100">
      <div className="flex flex-col gap-12 h-full">
        {/* Status, Latency, State, Title */}
        <div className="flex flex-col gap-2">
          <p className="font-bold text-xl">Connection</p>
          <ConnectionStatus podId={currentPod} />
          <PodDisconnectError status={connectionStatus} podId={currentPod} />
          <Latency podId={currentPod} />
        </div>
        <PodState state={podState} />
        <div className="flex flex-col justify-start">
          <p className="font-bold text-xl">Controls</p>
          {/* <h1 className="text-4xl font-title font-black mt-2">Controls</h1> */}
          {POD_IDS.map((podId) => (
            <PodControls
              key={podId}
              podId={podId}
              show={currentPod === podId}
            />
          ))}
        </div>
      </div>
      <Logo />
    </main>
  );
};
