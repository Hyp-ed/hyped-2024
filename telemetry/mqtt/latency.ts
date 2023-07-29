// connect to mqtt

import { connect, MqttClient } from 'mqtt';

// connect to mqtt
const client: MqttClient = connect('mqtt://localhost:1883');

// subscribe to topic
client.subscribe('hyped/pod_1/latency/request');

// on message
client.on('message', async (topic: string, message: Buffer) => {
  console.log(`Received message from ${topic}: ${message.toString()}`);
  // sleep for 100ms
  // await new Promise((resolve) => setTimeout(resolve, 50));
  // publish to topic
  client.publish('hyped/pod_1/latency/response', message);
});
