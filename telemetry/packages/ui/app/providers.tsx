import { POD_IDS, PodId } from '@hyped/telemetry-constants';
import { LiveLogsProvider } from './context/live-logs';
import { MQTTProvider } from './context/mqtt';
import { PodsProvider } from './context/pods';
import { config } from './config';
import { QoS } from './types/mqtt';

export const Providers = ({ children }: { children: React.ReactNode }) => (
  <MQTTProvider broker={config.MQTT_BROKER} qos={config.MQTT_QOS as QoS}>
    <PodsProvider>
      <LiveLogsProvider>{children}</LiveLogsProvider>
    </PodsProvider>
  </MQTTProvider>
);
