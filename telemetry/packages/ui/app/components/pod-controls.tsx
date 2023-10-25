import { toast } from 'react-hot-toast';
import { useEffect, useState } from 'react';
import { PodState } from './pod-state';
import { Button } from '@/components/ui/button';
import { Label } from '@/components/ui/label';
import { Switch } from '@/components/ui/switch';
import { cn } from '@/lib/utils';
import {
  clamp,
  lower,
  raise,
  retract,
  startPod,
  stopPod,
  startHP,
  stopHP,
  tilt,
} from '@/controls/controls';
import { usePod } from '@/context/pods';
import { http } from 'openmct/core/http';
import { log } from '@/lib/logger';
import { ALL_POD_STATES } from '@hyped/telemetry-constants';
import {
  Ban,
  ChevronDown,
  ChevronUp,
  ChevronsDown,
  ChevronsUp,
  Plug,
  PlugZap,
  Rocket,
  Siren,
} from 'lucide-react';

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
      <div className="space-y-6">
        <div className="flex justify-between items-center">
          <Label htmlFor="active-suspension" className="text-sm">
            {preChargeLive ? 'MC: Live' : 'MC: Precharge'}
          </Label>
          <Switch
            id="pre-charge-live"
            onCheckedChange={togglePreChargeLive}
            disabled={
              !(
                podState === ALL_POD_STATES.IDLE ||
                podState === ALL_POD_STATES.READY ||
                podState === ALL_POD_STATES.STOPPED
              )
            }
          />
        </div>
        <div className="flex flex-col gap-2">
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
            <Rocket /> START
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
            <Siren /> STOP
          </Button>
          <Button
            className={cn(
              'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
              clamped && 'bg-blue-600 hover:bg-blue-700',
              !clamped && 'bg-[#535353] hover:bg-[#222222]',
            )}
            onClick={() => {
              if (clamped) retract(podId);
              else clamp(podId);
              setClamped(!clamped);
            }}
          >
            <Ban /> {clamped ? 'RETRACT BRAKES' : 'CLAMP BRAKES'}
          </Button>
          <Button
            className={cn(
              'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
              raised && 'bg-blue-600 hover:bg-blue-700',
              !raised && 'bg-[#535353] hover:bg-[#222222]',
            )}
            onClick={() => {
              if (raised) lower(podId);
              else raise(podId);
              setRaised(!raised);
            }}
          >
            {raised ? <ChevronsDown /> : <ChevronsUp />}
            {raised ? 'LOWER' : 'RAISE'}
          </Button>
          <Button
            className={cn(
              'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
              deadmanSwitch && 'bg-red-600 hover:bg-red-700',
              !deadmanSwitch && 'bg-[#535353] hover:bg-[#222222]',
            )}
            onClick={() => {
              if (deadmanSwitch) stopHP(podId);
              else startHP(podId);
              setDeadmanSwitch(!deadmanSwitch);
            }}
          >
            {deadmanSwitch ? <PlugZap /> : <Plug />}
            {deadmanSwitch ? 'HP ACTIVE' : 'HP INACTIVE'}
          </Button>
        </div>
      </div>
    </div>
  );
};
