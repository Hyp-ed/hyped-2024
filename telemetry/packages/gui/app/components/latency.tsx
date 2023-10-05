import { usePod } from '@/context/pods';
import { POD_CONNECTION_STATUS } from '@/types/PodConnectionStatus';
import {
  Tooltip,
  TooltipContent,
  TooltipProvider,
  TooltipTrigger,
  TooltipArrow,
} from '@/components/ui/tooltip';
import { LatencyChart } from './latency-chart';
import { cn } from '@/lib/utils';

/**
 * Displays the latency between the base station and the pod
 */
export const Latency = ({ podId }: { podId: string }) => {
  const { latency, connectionStatus, previousLatencies } = usePod(podId);

  return (
    <TooltipProvider>
      <Tooltip delayDuration={300}>
        <TooltipTrigger className="text-left">
          {connectionStatus === POD_CONNECTION_STATUS.CONNECTED && latency ? (
            <p>
              <span className="">Latency: </span>
              <span
                className={cn(
                  'text-sm',
                  latency <= 50 && 'text-green-500',
                  latency > 50 && 'text-orange-500',
                  latency > 100 && 'text-red-500',
                )}
              >
                {latency} ms
              </span>
            </p>
          ) : (
            <p>
              Latency: <span className="text-red-500">N/A</span>
            </p>
          )}
        </TooltipTrigger>
        <TooltipContent
          side="bottom"
          className="border-none bg-black shadow-2xl"
        >
          {/* <TooltipArrow width={15} height={8} /> */}
          <LatencyChart data={previousLatencies} />
        </TooltipContent>
      </Tooltip>
    </TooltipProvider>
  );
};
