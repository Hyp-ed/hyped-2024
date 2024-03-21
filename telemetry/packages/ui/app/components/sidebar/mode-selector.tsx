import { useState } from 'react';
import { POD_IDS, PodId, pods } from '@hyped/telemetry-constants';
import { Label } from '../ui/label';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '../ui/select';
import { useCurrentMode } from '@/context/pods';

/**
 * A pod selector component which allows us to select a pod to view/control.
 */
export const ModeSelector = () => {
  const { currentMode, setCurrentMode } = useCurrentMode();

  return (
    <div className="space-y-2">
      <Label htmlFor="mode-select">Select Mode:</Label>
      <Select
        value={getDisplayText(currentMode)}
        onValueChange={(v) => setCurrentMode(getModeIdFromDisplayText(v))}
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

  return Object.values(modeOptions).map((mode) => (
    <SelectItem key={mode} value={mode}>
      {mode}
    </SelectItem>
  ));
};

/**
 * Gets the text to display in the mode selector.
 */
const getDisplayText = (mode: string) => modeOptions[mode];

const getModeIdFromDisplayText = (displayText: string) => {
  const mode = Object.keys(modeOptions)
    .find((key) => modeOptions[key] === displayText);
  return mode as string;
}

interface Options {
  [key: string]: string
}

const modeOptions: Options = {
  ALL: 'All systems on',
  LEV: 'Levitation only',
  LIM: 'Propulsion only'
}