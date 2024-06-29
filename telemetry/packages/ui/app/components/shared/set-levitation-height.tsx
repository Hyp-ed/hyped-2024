import { log } from '@/lib/logger';
import { http } from 'openmct/core/http';
import { useState } from 'react';
import { Input } from '@/components/ui/input';
import { Button } from '@/components/ui/button';

/**
 * Sets the levitation height of a pod.
 * @param podId The ID of the pod to set the levitation height of.
 * @returns A component to set the levitation height of a pod.
 */
export const SetLevitationHeight = ({ podId }: { podId: string }) => {
  const [height, setHeight] = useState<number | null>(null);

  const setLevitationHeight = async () => {
    // Don't do anything if the height is invalid
    if (!height || height < 0) return;
    log(`Setting the levitation height of ${podId} to ${height}mm`);
    const url = `pods/${podId}/controls/levitation-height?height=${height}`;
    await http.post(url);
    // Clear the input
    setHeight(null);
  };

  return (
    <div className="flex flex-col gap-2">
      <p className="text-sm">Set levitation height (mm):</p>
      <div className="flex gap-2">
        <Input
          data-testid="height-input"
          type="number"
          min={0}
          value={height ?? ''}
          onChange={(e) => setHeight(Number(e.target.value))}
        />
        <Button
          data-testid="set-height-button"
          className="bg-white hover:bg-gray-200 text-black"
          onClick={setLevitationHeight}
        >
          Set
        </Button>
      </div>
    </div>
  );
};
