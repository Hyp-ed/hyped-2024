import { useState, ComponentType } from 'react';
import { getStateType, ALL_POD_STATES } from '@hyped/telemetry-constants';
import { cn } from '@/lib/utils';
import { styles } from '@/components/shared/pod-state';

type StateButtonProps = {
  onStateChange: (newState: string) => void;
};

export const StateButton: React.FC<StateButtonProps> = ({ onStateChange }) => {
  const { TEXT, UNKNOWN, FAILURE_BRAKING, SAFE, ...NODE_STATES } =
    ALL_POD_STATES;
  const states = Object.keys(NODE_STATES).concat(SAFE);
  const [state, setState] = useState(states[0]);

  const handleClick = (state: string) => {
    const nextState = states[(states.indexOf(state) + 1) % states.length];

    setState(nextState);
    onStateChange(nextState);
  };

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

// StateButton.propTypes = {
//   onStateChange: PropTypes.func.isRequired
// }