import { Button } from '@/components/ui/button';
import { CONTROLS, control } from '@/lib/controls';
import { cn } from '@/lib/utils';
import { ArrowUpFromLine, Rocket, Siren } from 'lucide-react';
import { SetLevitationHeight } from '../shared/set-levitation-height';
import { useState } from 'react';
import { Switch } from '../ui/switch';
import { Label } from '@radix-ui/react-label';

interface PodControlsProps {
  podId: string;
  show: boolean;
}

export const PodControls = ({ podId, show }: PodControlsProps) => {
  return (
    <div className={cn('mt-2 space-y-8', show ? 'block' : 'hidden')}>
      <div className="flex flex-col gap-2">
        <div className="flex flex-col gap-2">
          <SetLevitationHeight podId={podId} />
          <LevitateButton podId={podId} />
        </div>
        <LaunchButton podId={podId} />
        <EmergencyStopButton podId={podId} />
      </div>
    </div>
  );
};

const LevitateButton = ({ podId }: { podId: string }) => (
  <Button
    className={cn(
      'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
      'bg-blue-600 hover:bg-blue-700',
    )}
    onClick={() => {
      control(podId, CONTROLS.LEVITATE);
    }}
  >
    <ArrowUpFromLine /> LEVITATE
  </Button>
);

const LaunchButton = ({ podId }: { podId: string }) => {
  const [enabled, setEnabled] = useState(false);

  return (
    <div className="space-y-2 w-full">
      <div className="flex items-center space-x-2">
        <Switch id="enable" checked={enabled} onCheckedChange={setEnabled} />
        <Label htmlFor="enable">Enable</Label>
      </div>
      <Button
        className={cn(
          'w-full px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
          'bg-green-600 hover:bg-green-700 disabled:bg-gray-400 disabled:cursor-not-allowed',
        )}
        disabled={!enabled}
        onClick={() => control(podId, CONTROLS.START)}
      >
        <Rocket /> LAUNCH
      </Button>
    </div>
  );
};

const EmergencyStopButton = ({ podId }: { podId: string }) => (
  <Button
    className={cn(
      'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
      'bg-red-700 hover:bg-red-800',
    )}
    onClick={() => control(podId, CONTROLS.STOP)}
  >
    <Siren /> EMERGENCY STOP
  </Button>
);
