import {
  pods,
  Pod,
  RangeMeasurement,
  SensorData,
  LiveReading,
} from '../src/index';

// Extract and categorise relevant sensor data
export const measurements = (Object.values(pods) as Pod[]).reduce(
  (acc, pod) => (
    Object.entries(pod.measurements).forEach(([key, measurement]) => {
      if (measurement.format === 'enum') return;
      acc[key] = measurement;
      // Normalise sensor name to its category's base name, as this function is called only once for each sensor type
      // replace name with base name, effectively categoring sensors by type and function
      acc[key].name = key.replace(/_[^_]*\d$/, '');
    }),
    acc
  ),
  {} as Record<string, RangeMeasurement>,
);

// console.log(measurements)

/**
 * Gets an arbitrary initial value for each reading
 * Testing functionTo be replaced with user defined params fetched from GUI
 * @param data a key - value item from the measurements object
 * @returns initial value for a given sensor/measurement
 */
const getInitialValue = (data: RangeMeasurement): number => {
  // Define initial conditions
  const initialVals: Record<string, number> = {
    accelerometer: 0,
    acceleration: 0,
    displacement: 0,
    // velocity: measurements.velocity.limits.critical.high * 0.1, // initial velocity > 0 for continuity of logistic function
    velocity: 0.3, // m/s (this aligns closely with logistic curve y-intercept)
    pressure: data.name.endsWith('reservoir') ? 5 : 1,
    thermistor: 25,
    keyence: 0,
    hall_effect: 0,
    levitation_height: 0,
    power_line_resistance: 10,
  };

  // Set initial value based on sensor types defined above
  if (initialVals.hasOwnProperty(data.name)) {
    return initialVals[data.name];
  } else if (data.name.startsWith('pressure')) {
    // Pressure gauges are subdivided into push, pull, brake and reservoir with different initial values
    return initialVals.pressure;
  } else {
    // If the sensor is not recognised, return a random value within the critical limits
    console.log('Unrecognised sensor', data);
    const { low, high } = data.limits.critical;
    return Math.floor(Math.random() * (high - low)) + low;
  }
};

/**
 * Counts quantity of sensors of each type, categorised by the physical quantity(ies) they measure
 * @param podData Key - value item from the measurements object
 * @param currentKey Sensor key representing its unique ID
 * @returns Amount of sensors present of given type
 */
const countSensors = <T extends Pod['measurements']>(
  podData: T,
  currentKey: string,
): number => {
  return Object.values(podData).filter((sensor: T[keyof T]) => {
    return sensor.key.startsWith(currentKey) && !sensor.key.endsWith('avg');
  }).length;
};

// Create new object to store existing and supplemental sensor parameters for live data testing
export const sensorData: SensorData = Object.fromEntries(
  Object.entries(measurements)

    // Group sensors by type
    .reduce(
      (acc, [key, sensor]): any => {
        // Initialise set of unique sensor keys if it doesn't already exist
        if (!acc.seen) acc.seen = new Set();

        // Check if the sensor key has already been processed
        if (!acc.seen.has(sensor.type)) {
          acc.seen.add(sensor.type);
          // Add one key value pair for each sensor type
          // Each type holds all data on its constituent sensors
          acc.entries.push([sensor.type, sensor]);
        }

        return acc;
      },
      { seen: new Set(), entries: [] as [string, RangeMeasurement][] },
    )
    .entries // Add quantity and readings properties, the latter set to the sensors' initial conditions
    .map(([name, data]: [string, RangeMeasurement]) => [
      name,
      {
        ...data,
        quantity: countSensors(measurements, data.name),
        // Create object with a key-value pair for each measurement of a given sensor type
        readings: Object.fromEntries(
          Object.keys(measurements)
            .filter(
              (name) =>
                !name.endsWith('avg') && measurements[name].type == data.type,
            )
            .map((el) => [el, getInitialValue(measurements[el])]),
        ),
      } as LiveReading,
    ]),
);

// console.log(sensorData);

// Parameter storing distance of track, once finsih point is reached program will end
export const trackLength = measurements.displacement.limits.critical.high;
