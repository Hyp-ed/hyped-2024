export type PodStateType = keyof typeof ALL_POD_STATES;

export const FAILURE_STATES = {
  FAILURE_BRAKING: 'FAILURE_BRAKING',
  FAILURE_STOPPED: 'FAILURE_STOPPED',
  FAILURE_CALIBRATING: 'FAILURE_CALIBRATING',
} as const;

export const PASSIVE_STATES = {
  IDLE: 'IDLE',
  CALIBRATING: 'CALIBRATING',
  READY: 'READY',
  STOPPED: 'STOPPED',
  OFF: 'OFF',
} as const;

export const ACTIVE_STATES = {
  ACCELERATING: 'ACCELERATING',
  NOMINAL_BRAKING: 'NOMINAL_BRAKING',
} as const;

export const NULL_STATES = {
  UNKNOWN: 'UNKNOWN',
} as const;

export const ALL_POD_STATES = {
  ...FAILURE_STATES,
  ...PASSIVE_STATES,
  ...ACTIVE_STATES,
  ...NULL_STATES,
};

export const ALL_POD_STATE_TYPES = [
  'FAILURE',
  'PASSIVE',
  'ACTIVE',
  'NULL',
] as const;

export const getStateType = (
  state: string,
): (typeof ALL_POD_STATE_TYPES)[number] => {
  if (FAILURE_STATES[state as keyof typeof FAILURE_STATES]) {
    return 'FAILURE';
  } else if (PASSIVE_STATES[state as keyof typeof PASSIVE_STATES]) {
    return 'PASSIVE';
  } else if (ACTIVE_STATES[state as keyof typeof ACTIVE_STATES]) {
    return 'ACTIVE';
  } else if (NULL_STATES[state as keyof typeof NULL_STATES]) {
    return 'NULL';
  } else {
    throw new Error(`Unknown state: ${state}`);
  }
};
