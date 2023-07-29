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

interface PodControlsProps {
  podId: string;
  show: boolean;
}

export const PodControls = ({ podId, show }: PodControlsProps) => {
  const { podState } = usePod(podId);

  const [clamped, setClamped] = useState(false);
  const [raised, setRaised] = useState(false);
  const [deadmanSwitch, setDeadmanSwitch] = useState(false);
  const [stopped, setStopped] = useState(true);
  const [preChargeLive, setPreChargeLive] = useState(false);

  /**
   * Toggle the precharge/live state of the mc
   * @param value Whether the mc should be live or precharge
   */
  const togglePreChargeLive = (value: boolean) => {
    setPreChargeLive(value);
    if (value) http.post(`pods/${podId}/controls/live-mc`);
    else http.post(`pods/${podId}/controls/pre-charge-mc`);
  };

  // Display notification when the pod state changes
  useEffect(() => {
    toast(`Pod state changed: ${podState}`);
    log(`Pod state changed: ${podState}`, podId);
  }, [podState]);

  return (
    <div className={cn('my-8 space-y-8', show ? 'block' : 'hidden')}>
      <PodState state={podState} />
      <div className="space-y-6">
        <div className="flex justify-between items-center">
          <Label htmlFor="active-suspension">
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
          {stopped ? (
            <Button
              className={cn(
                'px-4 py-10 rounded-md shadow-lg transition text-white text-3xl font-bold',
                'bg-green-600 hover:bg-green-700',
              )}
              onClick={() => {
                startPod(podId);
                setStopped(false);
              }}
            >
              START RUN
            </Button>
          ) : (
            <Button
              className={cn(
                'px-4 py-10 rounded-md shadow-lg transition text-white text-3xl font-bold',
                'bg-red-700 hover:bg-red-800',
              )}
              onClick={() => {
                stopPod(podId);
                setStopped(true);
              }}
            >
              STOP RUN
            </Button>
          )}
          <Button
            className={cn(
              'px-4 py-10 rounded-md shadow-lg transition text-white text-3xl font-bold',
              clamped && 'bg-blue-600 hover:bg-blue-700',
              !clamped && 'bg-gray-600 hover:bg-gray-700',
            )}
            onClick={() => {
              if (clamped) retract(podId);
              else clamp(podId);
              setClamped(!clamped);
            }}
          >
            {clamped ? 'Retract Brakes' : 'Clamp Brakes'}
          </Button>
          <Button
            className={cn(
              'px-4 py-10 rounded-md shadow-lg transition text-white text-3xl font-bold',
              raised && 'bg-blue-600 hover:bg-blue-700',
              !raised && 'bg-gray-600 hover:bg-gray-700',
            )}
            onClick={() => {
              if (raised) lower(podId);
              else raise(podId);
              setRaised(!raised);
            }}
          >
            {raised ? 'Lower Pod' : 'Raise Pod'}
          </Button>
          <Button
            className={cn(
              'px-4 py-10 rounded-md shadow-lg transition text-white text-3xl font-bold',
              deadmanSwitch && 'bg-red-600 hover:bg-red-700',
              !deadmanSwitch && 'bg-gray-600 hover:bg-gray-700',
            )}
            onClick={() => {
              if (deadmanSwitch) stopHP(podId);
              else startHP(podId);
              setDeadmanSwitch(!deadmanSwitch);
            }}
          >
            {deadmanSwitch ? 'HP Active' : 'HP Inactive'}
          </Button>

          <Button
            className={cn(
              'px-4 py-10 rounded-md shadow-lg transition text-white text-3xl font-bold',
              deadmanSwitch && 'bg-red-600 hover:bg-red-700',
              !deadmanSwitch && 'bg-gray-600 hover:bg-gray-700',
            )}
            onClick={() => {
              tilt(podId);
            }}
          >
            Tilt
          </Button>
        </div>
      </div>
    </div>
  );
};
