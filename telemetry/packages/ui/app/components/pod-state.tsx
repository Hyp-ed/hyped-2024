import { cn } from '@/lib/utils';
import {
  PodStateType,
  FAILURE_STATES,
  PASSIVE_STATES,
  NULL_STATES,
  ACTIVE_STATES,
} from '@hyped/telemetry-constants';
import { Button } from './ui/button';
import {
  Dialog,
  DialogContent,
  DialogFooter,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from '@/components/ui/dialog';
import { StateMachineFlowChart } from './flow/flow-chart';

/**
 * Displays the current state of the pod.
 * Opens a dialog with the State Machine Flow Chart when clicked.
 * @param state The current state of the pod
 */
export const PodState = ({ state }: { state: PodStateType }) => (
  <Dialog>
    <DialogTrigger asChild>
      <Button
        className={cn(
          'px-3 py-2 rounded-md w-full justify-start',
          state in NULL_STATES ||
            (state in ACTIVE_STATES &&
              'bg-gray-600 hover:bg-gray-700 border-2 border-gray-800 text-white'),
          state in ACTIVE_STATES &&
            'bg-green-700 hover:bg-green-800 border-2 border-green-900 text-white',
          state in FAILURE_STATES &&
            'bg-red-700 hover:bg-red-800 border-2 border-red-900 text-white',
        )}
      >
        <p>
          STATE: <b>{state.toUpperCase()}</b>
        </p>
      </Button>
    </DialogTrigger>
    <DialogContent className="min-w-[85%]">
      <DialogHeader>
        <DialogTitle>State Machine Flow Chart</DialogTitle>
      </DialogHeader>
      <div className="flex flex-col gap-4">
        <h1>
          Current State is:{' '}
          <span
            className={cn(
              'font-bold uppercase',
              state in NULL_STATES || (state in PASSIVE_STATES && 'text-white'),
              state in ACTIVE_STATES && 'text-green-600',
              state in FAILURE_STATES && 'text-red-600',
            )}
          >
            {state}
          </span>
        </h1>
        <StateMachineFlowChart currentState={state} />
      </div>
      <DialogFooter>
        <DialogTrigger asChild>
          <Button>Close</Button>
        </DialogTrigger>
      </DialogFooter>
    </DialogContent>
  </Dialog>
);
