export type PodStateType = keyof typeof ALL_POD_STATES;

export const FAILURE_STATES = {
  FAILURE_BRAKING: 'kFailureBrake',
  FAILURE: 'kFailure',
} as const;

export const PASSIVE_STATES = {
  IDLE: 'kIdle',
  CALIBRATE: 'kCalibrate',
  SAFE: 'kSafe',
} as const;

export const ACTIVE_STATES = {
  PRECHARGE: 'kPrecharge',
  READY_FOR_LEVITATION: 'kReadyForLevitation',
  BEGIN_LEVITATION: 'kBeginLevitation',
  LEVITATING: 'kLevitating',
  READY_FOR_LAUNCH: 'kReady',
  ACCELERATE: 'kAccelerate',
  LIM_BRAKE: 'kLimBrake',
  FRICTION_BRAKE: 'kFrictionBrake',
  STOP_LEVITATION: 'kStopLevitation',
  STOPPED: 'kStopped',
  BATTERY_RECHARGE: 'kBatteryRecharge',
  CAPACITOR_DISCHARGE: 'kCapacitorDischarge',
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

export type PodStateCategoryType = (typeof ALL_POD_STATE_TYPES)[number];

export const getStateType = (
  state: string,
): (typeof ALL_POD_STATE_TYPES)[number] => {
  if (FAILURE_STATES[state as keyof typeof FAILURE_STATES]) return 'FAILURE';
  if (PASSIVE_STATES[state as keyof typeof PASSIVE_STATES]) return 'PASSIVE';
  if (ACTIVE_STATES[state as keyof typeof ACTIVE_STATES]) return 'ACTIVE';
  if (NULL_STATES[state as keyof typeof NULL_STATES]) return 'NULL';
  throw new Error(`Unknown state: ${state}`);
};
