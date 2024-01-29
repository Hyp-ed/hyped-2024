export const accelerometerCommon = {
  format: 'float',
  type: 'motion',
  unit: 'm/s²',
  limits: {
    critical: {
      low: -150,
      high: 150,
    },
  },
  rmsNoise: 16.25 * 10^(-3), // RMS rmsNoise [mg] at ±15g range (~ ±150m/s^2)
  sampling_time: 1000
} as const;

// datasheet: https://www.st.com/en/mems-and-sensors/stts22h.html#st_description_sec-nav-tab
export const thermistorCommon = {
  format: 'float',
  type: 'thermistor',
  unit: '°C',
  limits: {
    critical: {
      low: -40,
      high: 125,
    },
    warning: {
      low: 20,
      high: 100,
    },
  },
  rmsNoise: 0.05, // RMS rmsNoise
  sampling_time: 500 // test value. Datasheet specifies clock frequency range as (10 - 400 kHz)
} as const;

export const pressureCommon = {
  format: 'float',
  type: 'pressure',
  unit: 'bar',
  rmsNoise: 1 * 10^(-3), // placeholder estimate of 1 mbar, to be confirmed with datasheet when chosen sensor confirmed
  sampling_time: 500
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
  rmsNoise: 0.5, // placeholder guesstimate, waiting on datasheet
  sampling_time: 500
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
  rmsNoise: 0,
  sampling_time: 500
} as const;