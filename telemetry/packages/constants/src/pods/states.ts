export type PodStateType = (typeof ALL_POD_STATES)[keyof typeof ALL_POD_STATES];

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
  state: any,
): (typeof ALL_POD_STATE_TYPES)[number] => {
  if (Object.values(FAILURE_STATES).includes(state)) return 'FAILURE';
  if (Object.values(PASSIVE_STATES).includes(state)) return 'PASSIVE';
  if (Object.values(ACTIVE_STATES).includes(state)) return 'ACTIVE';
  if (Object.values(NULL_STATES).includes(state)) return 'NULL';
  throw new Error(`Unknown state: ${state}`);
};
