import { MqttSender } from './mqtt-sender';

/**
 * TODO: in progress
 * Debug view. Contains components for debugging.
 * Will include:
 * - Full set of controls for pod
 * - Details connection statuses and latencies (connection to MQTT broker, connection to pod, etc.)
 * - Custom MQTT message sender
 * @returns The debug view
 */
export const DebugView = () => {
  return (
    <div className="grid grid-cols-2">
      <div></div>
      <MqttSender />
    </div>
  );
};
