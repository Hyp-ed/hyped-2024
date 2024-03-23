/**
 * This file is only used for debugging purposes and is not included in the final build.
 */
import { useEffect, useState } from 'react';
import {
  getStateType,
  ALL_POD_STATES,
  PodStateType,
  ModeType,
  MODE_INACTIVE_STATES,
} from '@hyped/telemetry-constants';
import { cn } from '@/lib/utils';
import { styles } from '@/components/shared/pod-state';

type StateButtonProps = {
  onStateChange: (newState: PodStateType) => void;
  mode: ModeType;
};

export const StateButton: React.FC<StateButtonProps> = ({
  onStateChange,
  mode,
}) => {
  const { TEXT, UNKNOWN, FAILURE_BRAKING, SAFE, ...NODE_STATES } =
    ALL_POD_STATES;
  const states: PodStateType[] = [...Object.values(NODE_STATES), SAFE].filter(
    (node) => {
      return !MODE_INACTIVE_STATES[mode].includes(node);
    },
  );
  const [state, setState] = useState(states[0]);

  const handleClick = (state: PodStateType) => {
    const nextState = states[(states.indexOf(state) + 1) % states.length];
    setState(nextState);
    onStateChange(nextState);
  };

  /**
   * Reset state progression when mode is changed
   */
  useEffect(() => {
    setState(states[0]);
    onStateChange(states[0]);
  }, [mode]);

  return (
    <div className={cn('flex justify-between px-40')}>
      <span className={cn('truncate w-50')}>{state}</span>
      <button
        className={cn(
          'p-3 rounded-md max-w-max flex-grow',
          styles[getStateType(state)],
        )}
        onClick={() => handleClick(state)}
      >
        Next State
      </button>
    </div>
  );
};
