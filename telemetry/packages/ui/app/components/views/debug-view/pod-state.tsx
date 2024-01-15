import { usePod } from '@/context/pods';
import { cn } from '@/lib/utils';
import {
  ACTIVE_STATES,
  FAILURE_STATES,
  NULL_STATES,
  pods,
} from '@hyped/telemetry-constants';
import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from '@/components/ui/card';
import { CircleDashed } from 'lucide-react';

const styles = {
  ACTIVE_STATES: 'bg-green-700 border-2 border-green-900 text-white',
  FAILURE_STATES: 'bg-red-700 border-2 border-red-900 text-white',
  NULL_STATES: 'bg-gray-600 border-2 border-gray-800 text-white',
};

export const PodState = ({ podId }: { podId: string }) => {
  const { podState: state, name } = usePod(podId);

  return (
    <Card className="border-none">
      <CardHeader>
        <CardTitle className="flex gap-2">
          <CircleDashed /> Pod State
        </CardTitle>
        <CardDescription>The current state of {name}</CardDescription>
      </CardHeader>
      <CardContent>
        <p
          className={cn(
            'px-3 py-2 rounded-md max-w-max justify-start font-bold uppercase',
            state in ACTIVE_STATES && styles.ACTIVE_STATES,
            state in FAILURE_STATES && styles.FAILURE_STATES,
            state in NULL_STATES && styles.NULL_STATES,
          )}
        >
          {state}
        </p>
      </CardContent>
    </Card>
  );
};
