// import types & pod data
import { 
  pods,
  Pod,
  Measurement,
  RangeMeasurement,
  LiveReading,
  LiveData,
  InitialConditions,
} from './src/index'

// data gen simulation utilities and complentary files
import sensors from './sensors';
import { DataManager } from './data-manager';


// TS error not recognising mqtt, will fix later
// import mqtt from 'mqtt';

// const client = mqtt.connect('mqtt://localhost:1883');

/**
 * Gets all of the measurements from the `pods.ts` file that we want to generate data for. (Currently excludes enum measurements)
 * Optionally could include a whitelist/blacklist of measurements to generate data for.
 */
export const measurements = (Object.values(pods) as Pod[]).reduce(
  (acc, pod) => (
    Object.entries(pod.measurements).forEach(([key, measurement]) => {
      if (measurement.format === 'enum') return;
      acc[key] = measurement;
    }),
    acc
  ),
  {} as Record<string, RangeMeasurement>,
);

export const sensorData: LiveData = {}

// helper functions
/**
 * counts quantity of sensors of that type, categorised by the variable of measurement
 * @param podData a key - value item from the measurements object
 * @param currentKey the sensor data's key (effectively its unique ID)
 * @returns sensor quantity
 */
const countSensors = <T extends Pod['measurements']>(podData: T, currentKey: string): number => {
  return Object.values(podData).filter( (sensor: Measurement) => {
    return sensor.key.startsWith(currentKey) && !sensor.key.endsWith('avg');
  }).length;
}

/**
 * Gets an arbitrary initial value for each reading
 * Testing function - To be replaced with user defined params fetched from GUI
 * @param podData a key - value item from the measurements object
 * @param currentKey the sensor data's key (effectively its unique ID)
 * @returns initial value for a given sensor/measurement
 */
const getInitialValue = <T extends Pod['measurements']>(data: RangeMeasurement): number => {
  switch(data.key) {
    case 'accelerometer':
    case 'acceleration':
    case 'displacement':
    case 'hall_effect':
    case 'levitation_height':
    case 'keyence':
      return 0;
    case 'velocity':
      // Initial velocity (t = 0) must be > 0 by definition, i.e. t = 0 being the start of the run
      // This also aids in smoothly transitioning into the asymptotic logistic curve velocity follows
      return data.limits.critical.high * 0.1;
    case 'thermistor':
      return 25;
    case 'power_line_resistance':
      return 10;
    default: 
      if (data.key.startsWith('pressure')) { break; }
    }
  // pneumatic and brake pressure
  if (data.key.match(/(push|pull|brakes[^s]) ('$1')/)) { return 1; }
  // reservoir pressure
  if (data.key.endsWith('reservoir')) { return 5; }
  
  else {
    console.log('Unrecognised sensor');
    const { low, high } = data.limits.critical;
    return Math.floor(Math.random() * (high - low)) + low;
  }
}

// create categorised object of sensor types and their respective measurements
for (const [name, data] of Object.entries(measurements)) {
  // don't overwrite if sensor already exists
  if (sensorData[data.type]) continue;
  data.key = data.key.replace(/_[^_]*\d$/, '');

  sensorData[data.type] = {
    ...data as Omit<RangeMeasurement, 'name'>,
    quantity: countSensors(measurements, data.key),
    liveData: Object.fromEntries(Object.keys(measurements)
      .filter( (name) => !name.endsWith('avg') && measurements[name].type == data.type)
      .map( el => [el, getInitialValue(data)])
    )
  } as Omit<LiveReading, 'name'>;
}

// instantiate DataManager instance
const dataManager = DataManager.getInstance(sensorData)

// instantiate sensors
const accelerometers = new sensors.Motion(sensorData['motion']);
const pressureGauges = new sensors.Pressure(sensorData['pressure']);
const thermistors = new sensors.Temperature(sensorData['temperature']);
const hallEffects = new sensors.HallEffect(sensorData['hall_effect']);
const keyence = new sensors.Keyence(sensorData['keyence']);
const resistance = new sensors.Resistance(sensorData['resistance']);
const levitation = new sensors.Levitation(sensorData['levitation']);

const generateDataSeries = (
  runTime: number,
  random: boolean = false,
  specific: false | string[] = false,
) => {
  const startData: LiveReading = JSON.parse(
    JSON.stringify(dataManager.getData()),
  );

  if (random) {
    for (const sensor of Object.values(startData)) {
      console.log(sensor);
    }
    return;
  }

  // sensor-specific functionality to be updated from old file and transferred here

}