import { Label } from '../ui/label';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '../ui/select';
import { useCurrentMode } from '@/context/pods';
import { MODES, ModeType } from '@hyped/telemetry-constants';

/**
 * A pod selector component which allows the selection of a mode of operation
 * for a pod run.
 */
export const ModeSelector = () => {
  const { currentMode, setCurrentMode } = useCurrentMode();

  return (
    <div className="space-y-2">
      <Label htmlFor="mode-select">Select Mode:</Label>
      <Select
        value={currentMode}
        onValueChange={(v: ModeType) => setCurrentMode(v)}
      >
        <SelectTrigger id="mode-select" className="w-full">
          <SelectValue />
        </SelectTrigger>
        <SelectContent>
          <ModeOptions />
        </SelectContent>
      </Select>
    </div>
  );
};

/**
 * Returns a primitive select element with the available pod run mode options.
 */
const ModeOptions = () => {
  return Object.keys(MODES).map((mode) => (
    <SelectItem key={mode} value={mode}>
      {mode}
    </SelectItem>
  ));
};
