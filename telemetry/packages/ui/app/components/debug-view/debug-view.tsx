import { PodState } from './pod-state';
import { ConnectionStatuses } from './connection-statuses/connection-statuses';
import { FullControls } from './full-controls';
import { MqttSender } from './mqtt-sender';
import {
  ResizableHandle,
  ResizablePanel,
  ResizablePanelGroup,
} from '@/components/ui/resizable';
import { useCurrentPod } from '@/context/pods';

/**
 * Debug view. Contains components for debugging.
 * Will include:
 * - Full set of controls for pod
 * - Details connection statuses and latencies (connection to MQTT broker, connection to pod, etc.)
 * - Custom MQTT message sender
 * @returns The debug view
 */
export const DebugView = () => {
  const { currentPod: podId } = useCurrentPod();

  return (
    <ResizablePanelGroup direction="vertical">
      <ResizablePanel defaultSize={20}>
        <ResizablePanelGroup direction="horizontal" className="w-full h-full">
          <ResizablePanel
            defaultSize={50}
            className="flex items-center"
          ></ResizablePanel>
          <ResizableHandle withHandle />
          <ResizablePanel defaultSize={50} className="flex items-center gap-2">
            <PodState podId={podId} />
          </ResizablePanel>
        </ResizablePanelGroup>
      </ResizablePanel>
      <ResizableHandle withHandle />
      <ResizablePanel defaultSize={30}>
        <FullControls podId={podId} />
      </ResizablePanel>
      <ResizableHandle withHandle />
      <ResizablePanel defaultSize={50}>
        <ResizablePanelGroup direction="horizontal" className="w-full h-full">
          <ResizablePanel
            defaultSize={60}
            className="flex items-center justify-center"
          >
            <ConnectionStatuses />
          </ResizablePanel>
          <ResizableHandle withHandle />
          <ResizablePanel
            defaultSize={40}
            className="flex items-center justify-center"
          >
            <MqttSender />
          </ResizablePanel>
        </ResizablePanelGroup>
      </ResizablePanel>
    </ResizablePanelGroup>
  );
};
