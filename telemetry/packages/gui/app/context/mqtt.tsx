import { createContext, useContext, useEffect, useState } from 'react';
import { IClientOptions, MqttClient } from 'mqtt/types/lib/client';
import { MqttPublish, MqttSubscribe, QoS } from '@/types/mqtt';
import {
  MQTTConnectionStatusType,
  MQTT_CONNECTION_STATUS,
} from '@/types/MQTTConnectionStatus';
import mqtt from 'mqtt/dist/mqtt';
import { MqttUnsubscribe } from '@/types/mqtt';
import { getTopic } from '@/lib/utils';
import { log } from '@/lib/logger';

type MQTTContextType = {
  client: MqttClient | null;
  publish: MqttPublish;
  subscribe: MqttSubscribe;
  unsubscribe: MqttUnsubscribe;
  mqttConnectionStatus: MQTTConnectionStatusType;
};

const MQTTContext = createContext<MQTTContextType | null>(null);

interface MQTTProviderProps {
  broker: string;
  qos: QoS;
  children: React.ReactNode;
}

/**
 * MQTT Context Provider.
 * Provides an MQTT client and functions to publish, subscribe and unsubscribe to MQTT topics.
 * Also provides the connection status of the MQTT client.
 */
export const MQTTProvider = ({ broker, qos, children }: MQTTProviderProps) => {
  const [client, setClient] = useState<MqttClient | null>(null);

  const [connectionStatus, setConnectionStatus] =
    useState<MQTTConnectionStatusType>(MQTT_CONNECTION_STATUS.UNKNOWN);

  /**
   * Connect to an MQTT broker
   * @param host The host to connect to
   * @param mqttOption The MQTT options
   */
  const mqttConnect = (host: string, mqttOption?: IClientOptions) => {
    log(`Connecting to MQTT broker: ${host}`);
    setConnectionStatus(MQTT_CONNECTION_STATUS.CONNECTING);
    const mqttClient = mqtt.connect(host, mqttOption);
    setClient(mqttClient);
  };

  // Connect to MQTT broker on mount
  useEffect(() => {
    mqttConnect(broker);
  }, []);

  // Handle client changes
  useEffect(() => {
    if (client) {
      client.on('connect', () => {
        log('MQTT client connected to broker');
        setConnectionStatus(MQTT_CONNECTION_STATUS.CONNECTED);
      });
      client.on('error', (err: any) => {
        log(`MQTT connection error: ${err}`);
        setConnectionStatus(MQTT_CONNECTION_STATUS.ERROR);
        client.end();
      });
      client.on('reconnect', () => {
        setConnectionStatus(MQTT_CONNECTION_STATUS.RECONNECTING);
      });
    } else {
      log("MQTT client doesn't exist, reconnecting...");
      mqttConnect(broker);
    }
  }, [client]);

  /**
   * Publish an MQTT message
   * @param topic The topic to publish to (will be prefixed with ```hyped/{podId}/```)
   * @param payload The payload to publish
   * @param podId The pod ID
   */
  const publish = (topic: string, payload: string, podId: string) => {
    const fullTopic = getTopic(topic, podId);
    if (!client) {
      log(`MQTT couldn't publish to ${fullTopic} because client is null`);
      return;
    }
    client.publish(fullTopic, payload, { qos }, (error) => {
      if (error) {
        log(`MQTT publish error: ${error}`);
      }
    });
  };

  /**
   * Subscribe to an MQTT topic
   * @param topic The topic to subscribe to (will be prefixed with ```hyped/{podId}/```)
   * @param podId The pod ID
   */
  const subscribe = (topic: string, podId: string) => {
    const fullTopic = getTopic(topic, podId);
    if (!client) {
      log(`MQTT couldn't subscribe to ${fullTopic} because client is null`);
      return;
    }
    client.subscribe(fullTopic, { qos });
  };

  /**
   * Unsubscribe from an MQTT topic
   * @param topic The topic to unsubscribe from (will be prefixed with ```hyped/{podId}/```)
   * @param podId The pod ID
   * @returns
   */
  const unsubscribe = (topic: string, podId: string) => {
    const fullTopic = getTopic(topic, podId);
    if (!client) {
      log(`MQTT couldn't unsubscribe from ${fullTopic} because client is null`);
      return;
    }
    client.unsubscribe(fullTopic);
  };

  return (
    <MQTTContext.Provider
      value={{
        client,
        publish,
        subscribe,
        unsubscribe,
        mqttConnectionStatus: connectionStatus,
      }}
    >
      {children}
    </MQTTContext.Provider>
  );
};

export const useMQTT = () => {
  const context = useContext(MQTTContext);
  if (!context) {
    throw new Error('useMQTT must be used within MQTTProvider');
  }
  return context;
};
