# instances

## individual limits accessed by `measurements[this.readings.[variable]].limits`

{
  Motion: Motion {
    name: 'Accelerometer 1',
    key: 'accelerometer',
    format: 'float',
    type: 'motion',
    unit: 'm/s²',
    limits: { critical: [Object] },
    rmsNoise: 0.01625,
    sampling_time: 1000,
    quantity: 4,
    readings: {
      accelerometer_1: 0,
      accelerometer_2: 0,
      accelerometer_3: 0,
      accelerometer_4: 0,
      displacement: 0,
      velocity: 5,
      acceleration: 0
    }
  },
  Pressure: Pressure {
    name: 'Pressure – Back Pull',
    key: 'pressure_back_pull',
    format: 'float',
    type: 'pressure',
    unit: 'bar',
    rmsNoise: 0.001,
    sampling_time: 500,
    limits: { critical: [Object], warning: [Object] },
    quantity: 8,
    readings: {
      pressure_back_pull: 1,
      pressure_front_pull: 1,
      pressure_front_push: 1,
      pressure_back_push: 1,
      pressure_brakes_reservoir: 5,
      pressure_active_suspension_reservoir: 5,
      pressure_front_brake: 1,
      pressure_back_brake: 1
    },
    losses: NaN
  },
  Temperature: Temperature {
    name: 'Thermistor 1',
    key: 'thermistor',
    format: 'float',
    type: 'temperature',
    unit: '°C',
    limits: { critical: [Object], warning: [Object] },
    rmsNoise: 0.05,
    sampling_time: 500,
    quantity: 12,
    readings: {
      thermistor_1: 25,
      thermistor_2: 25,
      thermistor_3: 25,
      thermistor_4: 25,
      thermistor_5: 25,
      thermistor_6: 25,
      thermistor_7: 25,
      thermistor_8: 25,
      thermistor_9: 25,
      thermistor_10: 25,
      thermistor_11: 25,
      thermistor_12: 25
    }
  },
  Hall_Effect: Hall_Effect {
    name: 'Hall Effect 1',
    key: 'hall_effect',
    format: 'float',
    type: 'hall_effect',
    unit: 'A',
    limits: { critical: [Object] },
    rmsNoise: 0.5,
    sampling_time: 500,
    quantity: 2,
    readings: { hall_effect_1: 0, hall_effect_2: 0 }
  },
  Keyence: Keyence {
    name: 'Keyence 1',
    key: 'keyence',
    format: 'integer',
    type: 'keyence',
    unit: 'number of stripes',
    limits: { critical: [Object] },
    rmsNoise: 0,
    sampling_time: 500,
    quantity: 2,
    readings: { keyence_1: 0, keyence_2: 0 }
  },
  Resistance: Resistance {
    name: 'Power Line Resistance',
    key: 'power_line_resistance',
    format: 'integer',
    type: 'resistance',
    unit: 'kΩ',
    limits: { critical: [Object] },
    rmsNoise: 1,
    sampling_time: 500,
    quantity: 1,
    readings: { power_line_resistance: 10 }
  },
  Levitation: Levitation {
    name: 'Levitation Height',
    key: 'levitation_height',
    format: 'float',
    type: 'levitation',
    unit: 'mm',
    limits: { critical: [Object] },
    rmsNoise: 2,
    sampling_time: 500,
    quantity: 1,
    readings: { levitation_height: 0 }
  }
}

# LiveData type
type LiveData = `Record<string, { [key: string]: number }>`
to collect readings under each sensor

e.g.

{
    motion: {
        accelerometer_1: 1.23;
        accelerometer_2: 1.28;
        ...
        displacement: 58.2;
    },
    pressure: {
        pressure_back_push: 1.34;
        ...
    },
    ...
}

# StoredData type
type StoredData = Record<number, LiveData> // number references timestamp

e.g. {
    0.5
}



# currentData (dataManager.data)

## similar to instances, but just dynamic object literals where 'readings' property is continually updated

{
  motion: {
    name: 'Accelerometer 1',
    key: 'accelerometer',
    format: 'float',
    type: 'motion',
    unit: 'm/s²',
    limits: { critical: [Object] },
    rmsNoise: 0.01625,
    sampling_time: 1000,
    quantity: 4,
    readings: {
      accelerometer_1: 0,
      accelerometer_2: 0,
      accelerometer_3: 0,
      accelerometer_4: 0,
      displacement: 0,
      velocity: 5,
      acceleration: 0
    }
  },
  pressure: {
    name: 'Pressure – Back Pull',
    key: 'pressure_back_pull',
    format: 'float',
    type: 'pressure',
    unit: 'bar',
    rmsNoise: 0.001,
    sampling_time: 500,
    limits: { critical: [Object], warning: [Object] },
    quantity: 8,
    readings: {
      pressure_back_pull: 1,
      pressure_front_pull: 1,
      pressure_front_push: 1,
      pressure_back_push: 1,
      pressure_brakes_reservoir: 5,
      pressure_active_suspension_reservoir: 5,
      pressure_front_brake: 1,
      pressure_back_brake: 1
    }
  },
  temperature: {
    name: 'Thermistor 1',
    key: 'thermistor',
    format: 'float',
    type: 'temperature',
    unit: '°C',
    limits: { critical: [Object], warning: [Object] },
    rmsNoise: 0.05,
    sampling_time: 500,
    quantity: 12,
    readings: {
      thermistor_1: 25,
      thermistor_2: 25,
      thermistor_3: 25,
      thermistor_4: 25,
      thermistor_5: 25,
      thermistor_6: 25,
      thermistor_7: 25,
      thermistor_8: 25,
      thermistor_9: 25,
      thermistor_10: 25,
      thermistor_11: 25,
      thermistor_12: 25
    }
  },
  hall_effect: {
    name: 'Hall Effect 1',
    key: 'hall_effect',
    format: 'float',
    type: 'hall_effect',
    unit: 'A',
    limits: { critical: [Object] },
    rmsNoise: 0.5,
    sampling_time: 500,
    quantity: 2,
    readings: { hall_effect_1: 0, hall_effect_2: 0 }
  },
  keyence: {
    name: 'Keyence 1',
    key: 'keyence',
    format: 'integer',
    type: 'keyence',
    unit: 'number of stripes',
    limits: { critical: [Object] },
    rmsNoise: 0,
    sampling_time: 500,
    quantity: 2,
    readings: { keyence_1: 0, keyence_2: 0 }
  },
  resistance: {
    name: 'Power Line Resistance',
    key: 'power_line_resistance',
    format: 'integer',
    type: 'resistance',
    unit: 'kΩ',
    limits: { critical: [Object] },
    rmsNoise: 1,
    sampling_time: 500,
    quantity: 1,
    readings: { power_line_resistance: 10 }
  },
  levitation: {
    name: 'Levitation Height',
    key: 'levitation_height',
    format: 'float',
    type: 'levitation',
    unit: 'mm',
    limits: { critical: [Object] },
    rmsNoise: 2,
    sampling_time: 500,
    quantity: 1,
    readings: { levitation_height: 0 }
  }
}


# sensors

{
  SensorLogic: [Function: SensorLogic] {
    getRandomValue: [Function (anonymous)],
    addRandomNoise: [Function (anonymous)]
  },
  Motion: [Function: Motion],
  Pressure: [Function: Pressure],
  Temperature: [Function: Temperature],
  Hall_Effect: [Function: Hall_Effect],
  Keyence: [Function: Keyence],
  Resistance: [Function: Resistance],
  Levitation: [Function: Levitation]
}