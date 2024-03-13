import { StateMachineFlowChart } from './flow-chart';
import { PodState } from '../../shared/pod-state';
import { useCurrentPod } from '@/context/pods';

/**
 * The state machine view which displays the current state of the pod and the flow chart describing the state machine.
 */
export const StateMachine = () => {
  const {
    pod: { id, podState },
  } = useCurrentPod();

  return (
    <div className="h-full">
      {/* <PodState podId={id} /> */}
      <StateMachineFlowChart currentState={podState} />
    </div>
  );
};
