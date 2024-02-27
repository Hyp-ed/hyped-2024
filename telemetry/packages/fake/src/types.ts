import { RangeMeasurement } from '.';

// for the variables representing physical sensors, not derived measurements
// export type LiveReading = Omit<RangeMeasurement, 'name'> & {
export type LiveReading = RangeMeasurement & {
  quantity: number; // don't think this is used anywhere or needed, just use .length
  // unique variable readinfgs it provides; e.g. accelerometer gives values for acceleration, displacement and velocity (latter two indirectly)
  readings: Readings;
};

export type SensorData = Record<string, LiveReading>;

// sensor's output readings
export type Readings = {
  [measurement: string]: number;
};

// all sensor's readings data
export type ReadingsMap = {
  [sensor: string]: Readings;
};

// transient data with indices representing timesteps
export type RunData = ReadingsMap[];

// return type for sensor class instantiation
export type SensorInstance<T extends new (...args: any[]) => any> =
  InstanceType<T>;
