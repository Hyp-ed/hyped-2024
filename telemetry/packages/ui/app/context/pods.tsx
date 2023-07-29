import {
  POD_CONNECTION_STATUS,
  PodConnectionStatusType,
} from '@/types/PodConnectionStatus';
import { createContext, useContext, useEffect, useState } from 'react';
import { useMQTT } from './mqtt';
import { MQTT_CONNECTION_STATUS } from '@/types/MQTTConnectionStatus';
import { getTopic } from '@/lib/utils';
import { ALL_POD_STATES, PodStateType } from '@hyped/telemetry-constants';
import { http } from 'openmct/core/http';

/**
 * The maximum latency before a pod is considered disconnected, in milliseconds
 */
const POD_MAX_LATENCY = 300;

/**
 * The latency at which a warning is sent to the server, in milliseconds
 */
const POD_WARNING_LATENCY = 100;

/**
 * The number of previous latencies to keep
 */
const NUM_PREVIOUS_LATENCIES = 50;

/**
 * The number of latencies to use to calculate average
 */
const NUM_LATENCIES_AVG = 10;

/**
 * The interval between latency messages, in milliseconds
 */
const LATENCY_REQUEST_INTERVAL = 100;

export type PreviousLatenciesType = {
  index: number;
  latency: number;
}[];

type PodsContextType = {
  [podId: string]: {
    connectionStatus: PodConnectionStatusType;
    previousLatencies?: PreviousLatenciesType;
    latency?: number;
    connectionEstablished?: Date;
    podState: PodStateType;
  };
};

const PodsContext = createContext<PodsContextType | null>(null);

function createPodsContextFromIds(podIds: string[]): PodsContextType {
  const podsContext: PodsContextType = {};
  for (const podId of podIds) {
    podsContext[podId] = {
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
  const [podsState, setPodsState] = useState<PodsContextType>(
    createPodsContextFromIds(podIds),
  );
  const [lastLatencyResponse, setLastLatencyResponse] = useState<number>();

  const { client, publish, subscribe, unsubscribe, mqttConnectionStatus } =
    useMQTT();

  useEffect(() => {
    // If we don't have an MQTT connection, set all pod connection statuses to disconnected
    if (mqttConnectionStatus !== MQTT_CONNECTION_STATUS.CONNECTED) {
      setPodsState((prevState) =>
        Object.fromEntries(
          Object.entries(prevState).map(([podId]) => [
            podId,
            {
              ...prevState[podId],
              connectionStatus: POD_CONNECTION_STATUS.DISCONNECTED,
              // reset previous latencies
              previousLatencies: [],
              // reset latency
              latency: undefined,
              // reset connection established
              connectionEstablished: undefined,
            },
          ]),
        ),
      );
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
            [podId]: {
              ...prevState[podId],
              connectionStatus: POD_CONNECTION_STATUS.DISCONNECTED,
              // reset previous latencies
              previousLatencies: [],
              // reset latency
              latency: undefined,
              // reset connection established
              connectionEstablished: undefined,
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
    const getLatency = (podId: string, topic: string, message: Buffer) => {
      if (topic === getTopic('state', podId)) {
        const newPodState = message.toString();
        const allowedStates = Object.values(ALL_POD_STATES);
        if (allowedStates.includes(newPodState as PodStateType)) {
          setPodsState((prevState) => ({
            ...prevState,
            [podId]: {
              ...prevState[podId],
              podState: newPodState as PodStateType,
            },
          }));
        }
      } else if (topic === getTopic('latency/response', podId)) {
        // calculate the latency
        const latency =
          new Date().getTime() -
          parseInt(JSON.parse(message.toString())['latency']);

        // send warning to the server if the latency is too high
        // send warning to the server if the latency is too high
        if (latency > POD_WARNING_LATENCY) {
          http.post(`pods/${podId}/warnings/latency`);
        }

        setLastLatencyResponse(new Date().getTime());

        // update the connection status
        setPodsState((prevState) => ({
          ...prevState,
          [podId]: {
            ...prevState[podId],
            connectionStatus: POD_CONNECTION_STATUS.CONNECTED,
            // maintain a list of the previous NUM_PREVIOUS_LATENCIES latencies
            previousLatencies: [
              ...(prevState[podId].previousLatencies || []).slice(
                -NUM_PREVIOUS_LATENCIES,
              ),
              {
                index: new Date().getTime(),
                latency,
              },
            ],
            // calculate the average latency from the last NUM_LATENCIES_AVG latencies
            latency: Math.round(
              (prevState[podId].previousLatencies || [])
                .slice(-NUM_LATENCIES_AVG)
                .reduce((acc, { latency }) => acc + latency, 0) /
                NUM_LATENCIES_AVG,
            ),
            // set the connection established time if it hasn't been set yet
            connectionEstablished:
              prevState[podId].connectionEstablished || new Date(),
          },
        }));
      }
    };

    podIds.map((podId) => {
      subscribe('latency/response', podId);
      subscribe('state', podId);
      client.on('message', (topic, message) =>
        getLatency(podId, topic, message),
      );
    });

    return () => {
      podIds.map((podId) => {
        client.off('message', (topic, message) =>
          getLatency(podId, topic, message),
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
  return context[podId];
};
