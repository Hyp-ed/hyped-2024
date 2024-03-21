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
 * A pod selector component which allows us to select a pod to view/control.
 */
export const ModeSelector = () => {
  const { currentMode, setCurrentMode }: { currentMode: ModeType, setCurrentMode: (modeType: ModeType) => void } = useCurrentMode();

  return (
    <div className="space-y-2">
      <Label htmlFor="mode-select">Select Mode:</Label>
      <Select
        value={getDisplayText(currentMode)}
        onValueChange={ (v: ModeType) => setCurrentMode(v) }
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
 * Returns the pod options for the pod selector by getting the display text for each pod in the `pods.ts` file.
 */
const ModeOptions = () => {
  // Get the display text for each pod in the `pods.ts` file

  return Object.keys(MODES).map((mode) => (
    <SelectItem key={mode} value={mode}>
      {mode}
    </SelectItem>
  ));
};

/**
 * Gets the text to display in the mode selector.
 */
const getDisplayText = (mode: ModeType) => MODES[mode];

// const getModeIdFromDisplayText = (displayText: ModeType) => {
//   return Object.keys(MODES)
//     .find((key) => MODES[key] === displayText);
// }

// interface Options {
//   [key: string]: string
// }

// const modeOptions: Options = {
//   ALL: 'All systems on',
//   LEV: 'Levitation only',
//   LIM: 'Propulsion only'
// }