import { POD_IDS } from '@hyped/telemetry-constants';
import { LiveLogsProvider } from './context/live-logs';
import { MQTTProvider } from './context/mqtt';
import { PodsProvider } from './context/pods';
import { env } from './env';
import { QoS } from './types/mqtt';

export const Providers = ({ children }: { children: React.ReactNode }) => (
  <MQTTProvider broker={env.MQTT_BROKER} qos={env.MQTT_QOS as QoS}>
    <PodsProvider podIds={(POD_IDS as unknown) as string[]}>
      <LiveLogsProvider>{children}</LiveLogsProvider>
    </PodsProvider>
  </MQTTProvider>
);
