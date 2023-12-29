import {
  Card,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from '@/components/ui/card';
import { usePod } from '@/context/pods';
import { PodConnectionStatusType } from '@/types/PodConnectionStatus';
import { pods } from '@hyped/telemetry-constants';
import { LatencyChart } from '@/components/latency-chart';

export const PodConnectionStatus = ({ podId }: { podId: string }) => {
  const {
    connectionStatus,
    connectionEstablished,
    latency,
    previousLatencies,
  } = usePod(podId);

  const statusComponentMap: Record<PodConnectionStatusType, React.ReactNode> = {
    CONNECTED: (
      <div className="flex gap-2 items-center">
        <div className="w-2 h-2 rounded-full bg-green-500 animate-[pulse_linear_1s_infinite]" />
        <p className="text-sm italic text-green-500">Connected</p>
      </div>
    ),
    DISCONNECTED: (
      <div className="flex gap-2 items-center">
        <div className="w-2 h-2 rounded-full bg-red-500" />
        <p className="text-sm italic text-red-500">Disconnected</p>
      </div>
    ),
    UNKNOWN: (
      <div className="flex gap-2 items-center">
        <div className="w-2 h-2 rounded-full bg-white animate-[pulse_linear_2s_infinite]" />
        <p className="text-sm italic text-white">Unknown</p>
      </div>
    ),
    ERROR: (
      <div className="flex gap-2 items-center">
        <div className="w-2 h-2 rounded-full bg-red-500 animate-[pulse_linear_0.2s_infinite]" />
        <p className="text-sm italic text-red-500">Error</p>
      </div>
    ),
  };

  return (
    <Card>
      <CardHeader>
        <CardTitle>
          {pods[podId].name} <span className="text-sm">({podId})</span>
        </CardTitle>
        <CardDescription>GUI connection to the MQTT broker</CardDescription>
      </CardHeader>
      <CardContent>
        <div className="flex flex-col gap-2">
          {statusComponentMap[connectionStatus]}
          <LatencyChart data={previousLatencies} />
        </div>
      </CardContent>
      <CardFooter></CardFooter>
    </Card>
  );
};
