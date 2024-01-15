import { Button } from '@/components/ui/button';
import { CONTROLS, control } from '@/lib/controls';
import { cn } from '@/lib/utils';
import { ArrowUpFromLine, Rocket, Siren } from 'lucide-react';
import { SetLevitationHeight } from './set-levitation-height';

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

const LaunchButton = ({ podId }: { podId: string }) => (
  <Button
    className={cn(
      'px-2 py-6 rounded-md shadow-lg transition text-white font-bold flex gap-2',
      'bg-green-600 hover:bg-green-700',
    )}
    onClick={() => control(podId, CONTROLS.START)}
  >
    <Rocket /> LAUNCH
  </Button>
);

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
