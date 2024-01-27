export const accelerometerCommon = {
  format: 'float',
  type: 'acceleration',
  unit: 'm/s²',
  limits: {
    critical: {
      low: -150,
      high: 150,
    },
  },
  timestep: 1000,
  initialValue: 0,
} as const;

export const thermistorCommon = {
  format: 'float',
  type: 'thermistor',
  unit: '°C',
  limits: {
    critical: {
      low: 15,
      high: 120,
    },
    warning: {
      low: 20,
      high: 100,
    },
  },
  timestep: 500,
  initialValue: 40,
} as const;

export const pressureCommon = {
  format: 'float',
  type: 'pressure',
  unit: 'bar',
  timestep: 500,
  initialValue: 1,
} as const;

export const hallEffectCommon = {
  format: 'float',
  type: 'hall_effect',
  unit: 'A',
  limits: {
    critical: {
      low: 0,
      high: 500,
    },
  },
  timestep: 250,
  initialValue: 0,
} as const;

export const keyenceCommon = {
  format: 'integer',
  type: 'keyence',
  unit: 'number of stripes',
  limits: {
    critical: {
      low: 0,
      high: 16,
    },
  },
  timestep: 100,
  initialValue: 0,
} as const;
