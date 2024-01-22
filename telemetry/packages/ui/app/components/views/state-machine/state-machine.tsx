import { StateMachineFlowChart } from './flow-chart';
import { PodState } from '../../shared/pod-state';
import { useCurrentPod } from '@/context/pods';

export const StateMachine = () => {
  const {
    pod: { id, podState },
  } = useCurrentPod();

  return (
    <div className="h-full pb-64">
      <PodState podId={id} />
      <StateMachineFlowChart currentState={podState} />
    </div>
  );
};
