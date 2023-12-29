import { Button } from '@/components/ui/button';
import { Label } from '@/components/ui/label';
import { Switch } from '@/components/ui/switch';
import { usePod } from '@/context/pods';
import {
  clamp,
  levitate,
  lower,
  raise,
  retract,
  startHP,
  startPod,
  stopHP,
  stopPod,
} from '@/controls/controls';
import { cn } from '@/lib/utils';
import { ALL_POD_STATES } from '@hyped/telemetry-constants';
import {
  ArrowUpFromLine,
  Ban,
  ChevronsDown,
  ChevronsUp,
  Plug,
  PlugZap,
  Rocket,
  Siren,
} from 'lucide-react';
import { http } from 'openmct/core/http';
import { useState } from 'react';

interface PodControlsProps {
  podId: string;
  show: boolean;
}

export const PodControls = ({ podId, show }: PodControlsProps) => {
  const [clamped, setClamped] = useState(false);
  const [raised, setRaised] = useState(false);
  const [deadmanSwitch, setDeadmanSwitch] = useState(false);
  const [stopped, setStopped] = useState(true);
  const [preChargeLive, setPreChargeLive] = useState(false);

  const { podState } = usePod(podId);

  /**
   * Toggle the precharge/live state of the mc
   * @param value Whether the mc should be live or precharge
   */
  const togglePreChargeLive = (value: boolean) => {
    setPreChargeLive(value);
    if (value) http.post(`pods/${podId}/controls/live-mc`);
    else http.post(`pods/${podId}/controls/pre-charge-mc`);
  };

  return (
    <div className={cn('mt-2 space-y-8', show ? 'block' : 'hidden')}>
      <div className="flex flex-col gap-2">
        <Button
          className={cn(
            'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
            'bg-blue-600 hover:bg-blue-700',
          )}
          onClick={() => {
            levitate(podId);
          }}
        >
          <ArrowUpFromLine /> LEVITATE
        </Button>
        <Button
          className={cn(
            'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
            'bg-green-600 hover:bg-green-700',
          )}
          onClick={() => {
            startPod(podId);
            setStopped(false);
          }}
        >
          <Rocket /> LAUNCH
        </Button>
        <Button
          className={cn(
            'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
            'bg-red-700 hover:bg-red-800',
          )}
          onClick={() => {
            stopPod(podId);
            setStopped(true);
          }}
        >
          <Siren /> EMERGENCY STOP
        </Button>
      </div>
    </div>
  );
};
