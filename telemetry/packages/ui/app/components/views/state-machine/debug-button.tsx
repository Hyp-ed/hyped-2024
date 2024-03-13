import { useState, useEffect } from 'react';
import { Handle, NodeProps } from 'reactflow';
import { PodStateType, ALL_POD_STATES } from '@hyped/telemetry-constants';
import { cn } from '@/lib/utils';
import { styles } from '@/components/shared/pod-state';

const btnStyles = {
  fontWeight: 'bold',
  cursor: 'pointer',
  border: '1px solid white',
  'border-radius': '10px',
  padding: '10px',
  margin: '0 0 20px 20px',
};

type StateButtonProps = {
  onStateChange: (newState: string) => void;
}

export const StateButton: React.FC<StateButtonProps> = ({ onStateChange }) => {

  const states = Object.keys(ALL_POD_STATES);
  const [state, setState] = useState(states[0]);

  const handleClick = (state: string) => {
    const nextState = states[
      (states.indexOf(state) + 1) % states.length
    ];
    setState(nextState);
    onStateChange(nextState);
  };
  return (
    <button style={btnStyles} onClick={() => handleClick(state)}>
      Next State
    </button>
  )
};
