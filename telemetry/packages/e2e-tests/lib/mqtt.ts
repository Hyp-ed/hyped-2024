import mqtt from 'mqtt';

export const client = mqtt.connect('mqtt://localhost:1883');

type MqttMessageValidation = (receivedTopic: string, message: Buffer) => void;

/**
 * Validates a message received on an MQTT topic.
 * @param topic The topic the message will be send on
 * @param validate A validation function which will be called with the received topic and message values
 * @param timeout Time to wait (in ms) before giving up
 */
export async function validateMqttMessage(
  trigger: () => void,
  validate: MqttMessageValidation,
  timeout = 1000,
): Promise<void> {
  return new Promise(async (resolve, reject) => {
    const client = mqtt.connect('mqtt://localhost:1883');

    client.on('connect', async () => {
      await client.subscribeAsync('#');

      // Handle incoming messages
      client.on('message', (receivedTopic, message) => {
        validate(receivedTopic, message);
        resolve();
      });

      trigger();
    });

    // Timeout if the message is not received
    setTimeout(() => {
      reject(new Error(`Timeout waiting for message.`));
    }, timeout);
  });
}