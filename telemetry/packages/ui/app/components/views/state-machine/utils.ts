import {
  PodStateType,
  FAILURE_STATES,
  ACTIVE_STATES,
  PASSIVE_STATES,
  MODE_INACTIVE_STATES,
  ALL_POD_STATES,
  ModeType,
} from '@hyped/telemetry-constants';

/**
 * Returns the node type based on the state of the pod (active, failure, static)
 * @param state The PodState of the pod
 * @returns The state-corresponding node type
 */
export const getNodeType = (state: PodStateType) => {
  if (state in FAILURE_STATES) return 'FailureNode';
  if (state in PASSIVE_STATES) return 'PassiveNode';
  if (state in ACTIVE_STATES) return 'ActiveNode';
};

export const getActiveStates = ((mode: ModeType) => {
  return Object.values(ALL_POD_STATES).filter(state => !(state in MODE_INACTIVE_STATES[mode]));
});