// @ts-nocheck
/* eslint-disable no-console */
import mqtt from 'mqtt';
import random from 'random';
import { pods } from '@hyped/telemetry-constants';
import { Measurement } from '@hyped/telemetry-types';

const client = mqtt.connect('mqtt://localhost:1883');

const SENSOR_UPDATE_INTERVAL = 1000;

/**
 * Generates a value for a measurement based on it's previous value.
 * Uses the measurement's type to generate more sensible test data.
 * @param previousValue The previous value of the measurement
 * @param measurement The measurement to generate the value for
 * @returns A value for the measurement
 */
const generateValueForMeasurement = (
  previousValue: string,
  measurement: Measurement,
): string => {
  // Use the measurement's type (e.g. acceleration) to generate a sensible value
  switch (measurement.type) {
    case 'temperature':
      if (measurement.format !== 'float')
        throw new Error('Measurement type must be "float"');
      if (measurement.range === undefined) {
        throw new Error(
          'Temperature measurement must have a min and max value',
        );
      }

      // Generate a value that is within the measurement's range, and is within 2 degrees of the previous value
      const temp = parseFloat(previousValue) + random.float(-2, 2);
      if (temp < measurement.range.min) {
        return String(measurement.range.min);
      }

      if (temp > measurement.range.max) {
        return String(measurement.range.max);
      }

      return String(temp);

    case 'acceleration':
      if (measurement.format !== 'float')
        throw new Error('Measurement type must be "float"');
      if (measurement.range === undefined) {
        throw new Error(
          'Acceleration measurement must have a min and max value',
        );
      }

      if (parseFloat(previousValue) > measurement.range.max - 20) {
        return String(0);
      }

      if (parseFloat(previousValue) === 0) {
        return String(random.float(10, measurement.range.max));
      }

      return String(previousValue + random.float(-1, 1));

    case 'keyence':
      if (measurement.format !== 'integer')
        throw new Error('Measurement type must be "integer"');
      if (measurement.range === undefined) {
        throw new Error('Keyence measurement must have a min and max value');
      }

      const keyence =
        Math.random() > 0.8
          ? parseFloat(previousValue) + 1
          : parseFloat(previousValue);

      return String(keyence);

    case 'brake_feedback':
      if (measurement.format !== 'enum')
        throw new Error('Measurement type must be "enum"');
      if (measurement.enumerations === undefined) {
        throw new Error('Brake feedback measurement must have enumerations');
      }

      return String(measurement.enumerations[0].value);

    default:
      // Fall back to using the measurement's format (e.g. float) if the type is not specified above
      return generateValueByFormat(measurement);
  }
};

/**
 * Generates the value for a measurement based on it's format (e.g. float), within the measurement's range.
 * @param measurement The measurement to generate the value for
 * @returns A value for the measurement
 */
const generateValueByFormat = (measurement: Measurement): string => {
  switch (measurement.format) {
    case 'float':
      if (measurement.range === undefined) {
        throw new Error(
          'Measurement of type "float" must have a min and max value',
        );
      }

      return String(
        random.float(measurement.range.min, measurement.range.max).toFixed(2),
      );

    case 'integer':
      if (measurement.range === undefined) {
        throw new Error(
          'Measurement of type "integer" must have a min and max value',
        );
      }

      return String(random.int(measurement.range.min, measurement.range.max));

    default:
      throw new Error(
        'Measurement format not supported: ' + measurement.format,
      );
  }
};

const initialValues: {
  [key: string]: string;
} = {
  'temperature.shell_front': '40',
  'temperature.shell_middle': '45',
  'temperature.shell_back': '35',
  accelerometer: '0',
  keyence: '0',
  brake_feedback: '1',
};

client.on('connect', () => {
  console.log('CLIENT CONNECTED');

  Object.entries(pods).forEach(([podId, pod]) => {
    Object.entries(pod.measurements).forEach(([, measurement]) => {
      const previousValues = initialValues;
      setInterval(() => {
        // Get the previous value for the measurement, or generate a new one if it doesn't exist
        const previousValue =
          previousValues[measurement.key] || generateValueByFormat(measurement);

        // Generate a new value for the measurement
        const newValue = generateValueForMeasurement(
          previousValue,
          measurement,
        );
        previousValues[measurement.key] = newValue;

        // Publish the new value to the MQTT broker
        client.publish(
          `hyped/${podId}/measurement/${measurement.key}`,
          newValue,
          (err) => {
            if (err) {
              console.error('ERROR PUBLISHING', err);
            } else {
              console.log(`Published ${measurement.name} = ${newValue}`);
            }
          },
        );
        console.log(`Published ${measurement.name} = ${newValue}`);
      }, SENSOR_UPDATE_INTERVAL);
    });
  });
});
