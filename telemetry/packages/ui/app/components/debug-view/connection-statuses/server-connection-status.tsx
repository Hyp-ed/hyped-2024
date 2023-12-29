import { useMQTT } from '@/context/mqtt';
import { MQTTConnectionStatusType } from '@/types/MQTTConnectionStatus';
import {
  Card,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from '@/components/ui/card';
import { Radio, Server } from 'lucide-react';
import { config } from '@/config';

export const ServerConnectionStatus = () => {
  return (
    <Card>
      <CardHeader>
        <CardTitle className="flex gap-2">
          <Server /> Server
        </CardTitle>
        <CardDescription>
          GUI connection to the Telemetry Server
        </CardDescription>
      </CardHeader>
      <CardContent>TODO</CardContent>
      <CardFooter className="text-sm opacity-60">
        {config.SERVER_ENDPOINT}
      </CardFooter>
    </Card>
  );
};
