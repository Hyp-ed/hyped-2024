import { LiveLogsProvider } from './context/live-logs';
import { MQTTProvider } from './context/mqtt';
import { PodsProvider } from './context/pods';
import { QoS } from './types/mqtt';
import { ErrorProvider } from './context/errors';
import { env } from '@hyped/env';

/**
 * Provider for all the contexts.
 * @param children The children to render
 */
export const Providers = ({ children }: { children: React.ReactNode }) => (
  <ErrorProvider>
    <MQTTProvider
      broker={env.PUBLIC_MQTT_BROKER_HOST}
      qos={env.PUBLIC_MQTT_QOS as QoS}
    >
      <PodsProvider>
        <LiveLogsProvider>{children}</LiveLogsProvider>
      </PodsProvider>
    </MQTTProvider>
  </ErrorProvider>
);
