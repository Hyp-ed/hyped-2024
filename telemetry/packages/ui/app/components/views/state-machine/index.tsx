import { StateMachineFlowChart } from './flow-chart';
import { PodState } from '@/components/shared/pod-state';
import { useCurrentPod } from '@/context/pods';
// Debugging
import { StateButton } from './debug-button';
import { useState } from 'react';
import { PodStateType } from '@hyped/telemetry-constants';

/**
 * The state machine view which displays the current state of the pod and the flow chart describing the state machine.
 */
export const StateMachine = () => {
  const {
    pod: { id, podState },
  } = useCurrentPod();

  // Debugging
  const [state, setState]: [PodStateType, any] = useState('IDLE');
  const handleStateChange = (newState: string) => {
    setState(newState);
  }

  return (
    <div className="h-full">
      <PodState podId={id} />
      <StateButton onStateChange={handleStateChange}/>
      {/* <StateMachineFlowChart currentState={podState} /> */}
      <StateMachineFlowChart currentState={state} />
    </div>
  );
};
