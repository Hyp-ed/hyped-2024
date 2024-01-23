import { PodState } from '@/components/shared/pod-state';
import { Button } from '@/components/ui/button';
import { Label } from '@/components/ui/label';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { ALL_POD_STATES, PodStateType } from '@hyped/telemetry-constants';
import { http } from 'openmct/core/http';
import { useState } from 'react';

export const PodStateUpdater = ({ podId }: { podId: string }) => {
  const [podState, setPodState] = useState<PodStateType>(ALL_POD_STATES.IDLE);

  const publishPodState = () => {
    console.log(`Manually publishing pod state: ${podState}`);
    http.post(`pods/${podId}/state/set`, {
      body: JSON.stringify({ state: podState }),
      headers: {
        'content-type': 'application/json',
      },
    });
  };

  return (
    <div className="flex gap-8 w-full justify-center">
      <div className="min-w-max">
        <PodState podId={podId} />
      </div>
      <div className="space-y-2 w-[300px] my-auto">
        <Label htmlFor="pod-select">Set State:</Label>
        <div className="flex gap-2">
          <Select
            value={podState}
            onValueChange={(value) => setPodState(value as PodStateType)}
          >
            <SelectTrigger id="pod-select" className="w-full">
              <SelectValue />
            </SelectTrigger>
            <SelectContent>
              {Object.keys(ALL_POD_STATES).map((state) => (
                <SelectItem key={state} value={state}>
                  {state}
                </SelectItem>
              ))}
            </SelectContent>
          </Select>
          <Button onClick={publishPodState}>Set</Button>
        </div>
      </div>
    </div>
  );
};
