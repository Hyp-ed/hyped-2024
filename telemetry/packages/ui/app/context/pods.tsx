import {
  POD_CONNECTION_STATUS,
  PodConnectionStatusType,
} from '@/types/PodConnectionStatus';
import { createContext, useContext, useEffect, useState } from 'react';
import { useMQTT } from './mqtt';
import { MQTT_CONNECTION_STATUS } from '@/types/MQTTConnectionStatus';
import { getTopic } from '@/lib/utils';
import {
  ALL_POD_STATES,
  POD_IDS,
  PodStateType,
  pods,
} from '@hyped/telemetry-constants';
import { http } from 'openmct/core/http';

/**
 * The maximum latency before a pod is considered disconnected, in milliseconds
 */
const POD_MAX_LATENCY = 300 as const;

/**
 * The latency at which a warning is sent to the server, in milliseconds
 */
const POD_WARNING_LATENCY = 100 as const;

/**
 * The number of previous latencies to keep
 */
const NUM_PREVIOUS_LATENCIES = 50 as const;

/**
 * The number of latencies to use to calculate average
 */
const NUM_LATENCIES_AVG = 10 as const;

/**
 * The interval between latency messages, in milliseconds
 */
const LATENCY_REQUEST_INTERVAL = 100 as const;

/**
 * The default pod ID to use
 */
export const DEFAULT_POD_ID = POD_IDS[0];

export type PreviousLatenciesType = {
  index: number;
  latency: number;
}[];

type PodsContextType = {
  currentPod: string;
  setPod: (podId: string) => void;
  pods: {
    [podId: string]: {
      id: string;
      name: string;
      connectionStatus: PodConnectionStatusType;
      previousLatencies?: PreviousLatenciesType;
      latency?: number;
      connectionEstablished?: Date;
      podState: PodStateType;
    };
  };
};

const PodsContext = createContext<PodsContextType | null>(null);

function createPodsContextFromIds(podIds: string[]): PodsContextType {
  const podsContext: PodsContextType = {
    currentPod: DEFAULT_POD_ID,
    setPod: (_: string) => {},
    pods: {},
  };
  for (const podId of podIds) {
    podsContext.pods[podId] = {
      id: podId,
      name: pods[podId].name,
      connectionStatus: POD_CONNECTION_STATUS.UNKNOWN,
      podState: ALL_POD_STATES.UNKNOWN,
    };
  }
  return podsContext;
}

/**
 * MQTT Context Provider.
 * Provides an MQTT client and functions to publish, subscribe and unsubscribe to MQTT topics.
 * Also provides the connection status of the MQTT client.
 */
export const PodsProvider = ({
  podIds,
  children,
}: {
  podIds: string[];
  children: React.ReactNode;
}) => {
  const [podsState, setPodsState] = useState<PodsContextType>({
    ...createPodsContextFromIds(podIds),
    setPod: (podId: string) => {
      setPodsState((prevState: PodsContextType) => ({
        ...prevState,
        currentPod: podId,
      }));
    },
  });
  const [lastLatencyResponse, setLastLatencyResponse] = useState<number>();

  const { client, publish, subscribe, unsubscribe, mqttConnectionStatus } =
    useMQTT();

  useEffect(() => {
    // If we don't have an MQTT connection, set all pod connection statuses to disconnected
    if (mqttConnectionStatus !== MQTT_CONNECTION_STATUS.CONNECTED) {
      setPodsState((prevState: PodsContextType) => ({
        ...prevState,
        pods: Object.fromEntries(
          Object.entries(prevState.pods).map(([podId]) => [
            podId,
            {
              ...prevState.pods[podId],
              connectionStatus: POD_CONNECTION_STATUS.DISCONNECTED,
              previousLatencies: [], // reset previous latencies
              latency: undefined, // reset latency
              connectionEstablished: undefined, // reset connection established
            },
          ]),
        ),
      }));
    }
  }, [mqttConnectionStatus]);

  useEffect(() => {
    // send latency messages every LATENCY_INTERVAL milliseconds
    const interval = setInterval(() => {
      podIds.map((podId) => {
        publish(
          'latency/request',
          JSON.stringify({
            latency: new Date().getTime().toString(),
          }),
          podId,
        );
      });
    }, LATENCY_REQUEST_INTERVAL);
    return () => clearInterval(interval);
  }, [client]);

  useEffect(() => {
    const interval = setTimeout(() => {
      podIds.map((podId) => {
        if (!lastLatencyResponse) return;
        if (new Date().getTime() - lastLatencyResponse > POD_MAX_LATENCY) {
          setPodsState((prevState) => ({
            ...prevState,
            pods: {
              [podId]: {
                ...prevState.pods[podId],
                connectionStatus: POD_CONNECTION_STATUS.DISCONNECTED,
                // reset previous latencies
                previousLatencies: [],
                // reset latency
                latency: undefined,
                // reset connection established
                connectionEstablished: undefined,
              },
            },
          }));
        }
      });
    }, POD_MAX_LATENCY);
    return () => clearInterval(interval);
  }, [lastLatencyResponse]);

  // subscribe to latency messages and calculate latency
  useEffect(() => {
    if (!client) return;
    const processMessage = (podId: string, topic: string, message: Buffer) => {
      if (topic === getTopic('state', podId)) {
        console.log(podId);
        const newPodState = message.toString();
        console.log(newPodState);
        const allowedStates = Object.values(ALL_POD_STATES);
        if (allowedStates.includes(newPodState as PodStateType)) {
          setPodsState((prevState) => ({
            ...prevState,
            pods: {
              [podId]: {
                ...prevState.pods[podId],
                podState: newPodState as PodStateType,
              },
            },
          }));
        }
      } else if (topic === getTopic('latency/response', podId)) {
        // calculate the latency
        const latency =
          new Date().getTime() -
          parseInt(JSON.parse(message.toString())['latency']);

        // send warning to the server if the latency is too high
        if (latency > POD_WARNING_LATENCY) {
          http.post(`pods/${podId}/warnings/latency`);
        }

        setLastLatencyResponse(new Date().getTime());

        // update the connection status
        setPodsState((prevState) => ({
          ...prevState,
          pods: {
            [podId]: {
              ...prevState.pods[podId],
              connectionStatus: POD_CONNECTION_STATUS.CONNECTED,
              // maintain a list of the previous NUM_PREVIOUS_LATENCIES latencies
              previousLatencies: [
                ...(prevState.pods[podId].previousLatencies || []).slice(
                  -NUM_PREVIOUS_LATENCIES,
                ),
                {
                  index: new Date().getTime(),
                  latency,
                },
              ],
              // calculate the average latency from the last NUM_LATENCIES_AVG latencies
              latency: Math.round(
                (prevState.pods[podId].previousLatencies || [])
                  .slice(-NUM_LATENCIES_AVG)
                  .reduce((acc, { latency }) => acc + latency, 0) /
                  NUM_LATENCIES_AVG,
              ),
              // set the connection established time if it hasn't been set yet
              connectionEstablished:
                prevState.pods[podId].connectionEstablished || new Date(),
            },
          },
        }));
      }
    };

    podIds.map((podId) => {
      subscribe('latency/response', podId);
      subscribe('state', podId);
      client.on('message', (topic, message) =>
        processMessage(podId, topic, message),
      );
    });

    return () => {
      podIds.map((podId) => {
        client.off('message', (topic, message) =>
          processMessage(podId, topic, message),
        );
        unsubscribe('latency/response', podId);
        unsubscribe('state', podId);
      });
    };
  }, [client]);

  return (
    <PodsContext.Provider value={podsState}>{children}</PodsContext.Provider>
  );
};

export const usePod = (podId: string) => {
  const context = useContext(PodsContext);
  if (!context) {
    throw new Error('usePods must be used within PodsProvider');
  }
  // Get the pod connection status for the given podId
  return context.pods[podId];
};

export const usePods = () => {
  const context = useContext(PodsContext);
  if (!context) {
    throw new Error('usePods must be used within PodsProvider');
  }
  return {
    pods: context.pods,
  };
};

export const useCurrentPod = () => {
  const context = useContext(PodsContext);
  if (!context) {
    throw new Error('useCurrentPod must be used within PodsProvider');
  }
  return {
    currentPod: context.currentPod,
    pod: context.pods[context.currentPod],
    setPod: context.setPod,
  };
};
