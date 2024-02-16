import {
  Card,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from '@/components/ui/card';
import { MqttConnectionStatus } from './mqtt-connection-status';
import { PodConnectionStatus } from './pod-connection-statuses';
import {
  Carousel,
  CarouselContent,
  CarouselItem,
  CarouselNext,
  CarouselPrevious,
} from '@/components/ui/carousel';
import { Signal } from 'lucide-react';
import { POD_IDS, pods } from '@hyped/telemetry-constants';
import { ServerConnectionStatus } from './server-connection-status';

/**
 * Displays the connection statuses of the GUI, server, and pods.
 * @returns A Card component displaying the connection statuses of the GUI, server, and pods.
 */
export const ConnectionStatuses = () => {
  return (
    <Card className="border-none w-full">
      <CardHeader>
        <CardTitle className="flex gap-2">
          <Signal /> Connection Statuses
        </CardTitle>
        <CardDescription>
          Monitor connection statuses and latencies
        </CardDescription>
      </CardHeader>
      <CardContent>
        <Carousel
          opts={{
            align: 'start',
          }}
          className="mx-10"
        >
          <CarouselContent>
            <CarouselItem className="lg:basis-1/2 xl:basis-1/3">
              <MqttConnectionStatus />
            </CarouselItem>
            <CarouselItem className="lg:basis-1/2 xl:basis-1/3">
              <ServerConnectionStatus />
            </CarouselItem>
            {POD_IDS.map((podId) => (
              <CarouselItem className="lg:basis-1/2 xl:basis-1/3 text-white">
                <PodConnectionStatus podId={podId} />
              </CarouselItem>
            ))}
          </CarouselContent>
          <CarouselPrevious />
          <CarouselNext />
        </Carousel>
      </CardContent>
    </Card>
  );
};
