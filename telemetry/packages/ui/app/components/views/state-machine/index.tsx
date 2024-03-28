import { StateMachineFlowChart } from './flow-chart';
import { PodState } from '@/components/shared/pod-state';
import { useCurrentPod } from '@/context/pods';

import { StateButton } from './dev-components/debug-btn';
import React, { useState } from 'react';
import { PodStateType, ModeType } from '@hyped/telemetry-constants';
import { config } from '@/config';

/**
 * The state machine view which displays the current state of the pod and the flow chart describing the state machine.
 */
export const StateMachine = () => {
  const {
    pod: { id },
  } = useCurrentPod();

  // Debug state change buttom
  const [state, setState] = useState<PodStateType>('IDLE');
  const handleStateChange = (newState: PodStateType) => {
    setState(newState);
  };

  const [mode, setMode] = useState<ModeType>('ALL_SYSTEMS_ON');
  const handleModeChange = (newMode: ModeType) => {
    setMode(newMode);
  };

  return <StateMachineFlowChart 
      onModeChange={handleModeChange}
      currentState={state}
    />

  return (
    <div className="h-full">
      <PodState podId={id} />
      <StateButton onStateChange={handleStateChange} mode={mode} />
      <StateMachineFlowChart
        onModeChange={handleModeChange}
        currentState={state}
      />
    </div>
  );
};
