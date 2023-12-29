import { ConnectionStatuses } from './connection-statuses/connection-statuses';
import { FullControls } from './full-controls';
import { MqttSender } from './mqtt-sender';
import {
  ResizableHandle,
  ResizablePanel,
  ResizablePanelGroup,
} from '@/components/ui/resizable';

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
    <ResizablePanelGroup direction="vertical" className="h-full w-full">
      <ResizablePanel defaultSize={50}>
        <FullControls />
      </ResizablePanel>
      <ResizableHandle withHandle />
      <ResizablePanel defaultSize={50}>
        <ResizablePanelGroup direction="horizontal" className="w-full h-full">
          <ResizablePanel
            defaultSize={50}
            className="flex items-center justify-center"
          >
            <ConnectionStatuses />
          </ResizablePanel>
          <ResizableHandle withHandle />
          <ResizablePanel
            defaultSize={50}
            className="flex items-center justify-center"
          >
            <MqttSender />
          </ResizablePanel>
        </ResizablePanelGroup>
      </ResizablePanel>
    </ResizablePanelGroup>
  );
};
