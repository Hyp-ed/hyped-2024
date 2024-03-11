import { Edge, EdgeMarkerType, MarkerType } from 'reactflow';

// defines the arrow marker for the edges
const arrow: EdgeMarkerType = {
  type: MarkerType.Arrow,
  width: 32,
  height: 32,
  strokeWidth: 0.5,
};

export const edges: Edge[] = [
  {
    id: 'idle-calibrate',
    source: 'idle',
    target: 'calibrate',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'calibrate-precharge',
    source: 'calibrate',
    target: 'precharge',
    sourceHandle: 'right',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'precharge-ready-lev',
    source: 'precharge',
    target: 'ready-lev',
    sourceHandle: 'right',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'ready-lev-begin-lev',
    source: 'ready-lev',
    target: 'begin-lev',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'begin-lev-ready-launch',
    source: 'begin-lev',
    target: 'ready-launch',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'ready-launch-accelerate',
    source: 'ready-launch',
    target: 'accelerate',
    type: 'step',
    markerEnd: arrow,
  },
  //
  {
    id: 'accelerate-failure-braking',
    source: 'accelerate',
    target: 'failure-braking',
    sourceHandle: 'right',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'accelerate-lim-brake',
    source: 'accelerate',
    target: 'lim-brake',
    sourceHandle: 'bottom',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'lim-brake-failure-braking',
    source: 'lim-brake',
    target: 'failure-braking',
    sourceHandle: 'right',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'lim-brake-friction-brake',
    source: 'lim-brake',
    target: 'friction-brake',
    sourceHandle: 'bottom',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'friction-brake-failure-braking',
    source: 'friction-brake',
    target: 'failure-braking',
    sourceHandle: 'right',
    targetHandle: 'right',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'friction-brake-stop-lev',
    source: 'friction-brake',
    target: 'stop-lev',
    sourceHandle: 'bottom',
    type: 'step',
    markerEnd: arrow,
  },
  //
  {
    id: 'stop-lev-stopped',
    source: 'stop-lev',
    target: 'stopped',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'stopped-battery-recharge',
    source: 'stopped',
    target: 'battery-recharge',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'battery-recharge-capacitor-discharge',
    source: 'battery-recharge',
    target: 'capacitor-discharge',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'capacitor-discharge-safe',
    source: 'capacitor-discharge',
    target: 'safe',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'failure-braking-capacitor-discharge',
    source: 'failure-braking',
    target: 'capacitor-discharge',
    sourceHandle: 'bottom',
    targetHandle: 'right',
    type: 'step',
    markerEnd: arrow,
  },

  
  
  // old
  {
    id: 'nominal-braking-stopped',
    source: 'nominal-braking',
    target: 'stopped',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'nominal-braking-failure-braking',
    source: 'nominal-braking',
    target: 'failure-braking',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'stopped-off',
    source: 'stopped',
    target: 'off',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'failure-braking-failure-stopped',
    source: 'failure-braking',
    target: 'failure-stopped',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'failure-stopped-off',
    source: 'failure-stopped',
    target: 'off',
    targetHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
];
