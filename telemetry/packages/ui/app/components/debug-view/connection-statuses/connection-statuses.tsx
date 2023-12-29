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

export const ConnectionStatuses = () => {
  return (
    <Card className="border-none">
      <CardHeader>
        <CardTitle>Connection Statuses</CardTitle>
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
            <CarouselItem className="basis-1/2 h-full">
              {/* TODO: get pod ID from somewhere */}
              <PodConnectionStatus podId="pod_1" />
            </CarouselItem>
            <CarouselItem className="basis-1/2 h-full">
              <MqttConnectionStatus />
            </CarouselItem>
            <CarouselItem className="basis-1/2 h-full">
              <MqttConnectionStatus />
            </CarouselItem>
          </CarouselContent>
          <CarouselPrevious />
          <CarouselNext />
        </Carousel>
      </CardContent>
      <CardFooter></CardFooter>
    </Card>
  );
};
