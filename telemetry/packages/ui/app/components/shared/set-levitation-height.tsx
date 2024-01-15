import { log } from '@/lib/logger';
import { http } from 'openmct/core/http';
import { useState } from 'react';
import { Input } from '../ui/input';
import { Button } from '../ui/button';

export const SetLevitationHeight = ({ podId }: { podId: string }) => {
  const [height, setHeight] = useState<number | null>(null);

  const setHeightHandler = async () => {
    if (!height || height < 0) return;
    log(`Setting the levitation height of ${podId} to ${height}mm`);
    const url = `pods/${podId}/controls/levitation-height?height=${height}`;
    await http.post(url);
    // Clear
    setHeight(null);
  };

  return (
    <div className="flex flex-col gap-2">
      <p className="text-sm">Set levitation height (mm):</p>
      <div className="flex gap-2">
        <Input
          type="number"
          min={0}
          value={height ?? ''}
          onChange={(e) => setHeight(Number(e.target.value))}
        />
        <Button
          className="bg-white hover:bg-gray-200 text-black"
          onClick={setHeightHandler}
        >
          Set
        </Button>
      </div>
    </div>
  );
};
