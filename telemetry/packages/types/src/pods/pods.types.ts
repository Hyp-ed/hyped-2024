// common properties shared by all response variables
export type BaseMeasurement = {
  name: string;
  key: string;
  unit: string;
  type: string;
};

// range limits not to be exceeded
// some give warnings when reaching range limits
export type Limits = {
  warning?: {
    low: number;
    high: number;
  };
  critical: {
    low: number;
    high: number;
  };
};

// for the variables within ranges e.g. temperature
export type RangeMeasurement = BaseMeasurement & {
  format: 'float' | 'integer';
  limits: Limits;
  rms_noise: number; // RMS value from relevant datasheet
  sampling_time: number; // sensor timestep configuration (inverse of clock frequency)
};

// for rest of the variables that have N states enumerated by a number (0 or 1 generally) and descriptive string
export type EnumMeasurement = BaseMeasurement & {
  format: 'enum';
  enumerations: {
    value: number;
    string: string;
  }[];
};

// export type Measurement as union
export type Measurement = RangeMeasurement | EnumMeasurement;

// create Pod type
export type Pod = {
  name: string;
  id: string;
  measurements: Record<string, Measurement>;
};
