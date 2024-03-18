import { Edge, EdgeMarkerType, MarkerType } from 'reactflow';

// defines the arrow marker for the edges
const arrow: EdgeMarkerType = {
  type: MarkerType.Arrow,
  width: 32,
  height: 32,
  strokeWidth: 0.5,
};

const columns = {
  left: [
    'idle',
    'calibrate',
    'precharge',
    'ready-lev',
    'begin-lev',
    'ready-launch',
  ],
  center: ['accelerate', 'lim-brake', 'friction-brake'],
  right: [
    'stop-lev',
    'stopped',
    'battery-recharge',
    'capacitor-discharge',
    'safe',
  ],
};

export const getEdgeType = (source: string): [string, string] => {
  if (columns.left.includes(source)) return ['right', 'left'];
  if (columns.center.includes(source)) return ['left', 'left'];
  if (columns.right.includes(source)) return ['right', 'bottom'];
  return ['left', 'left'];
};

export const edges: Edge[] = [
  // Dynamic connection
  {
    id: 'precharge-failure-braking',
    source: 'precharge',
    target: 'failure-braking',
    sourceHandle: 'right',
    targetHandle: 'left',
    type: 'step',
    markerEnd: arrow,
  },
  // Static connections
  {
    id: 'idle-calibrate',
    source: 'idle',
    target: 'calibrate',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'calibrate-precharge',
    source: 'calibrate',
    target: 'precharge',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'precharge-ready-lev',
    source: 'precharge',
    target: 'ready-lev',
    sourceHandle: 'top',
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
    sourceHandle: 'top',
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
    id: 'accelerate-lim-brake',
    source: 'accelerate',
    target: 'lim-brake',
    sourceHandle: 'bottom',
    type: 'step',
    markerEnd: arrow,
  },
  // {
  //   id: 'lim-brake-failure-braking',
  //   source: 'lim-brake',
  //   target: 'failure-braking',
  //   sourceHandle: 'left',
  //   type: 'step',
  //   markerEnd: arrow,
  // },
  {
    id: 'lim-brake-friction-brake',
    source: 'lim-brake',
    target: 'friction-brake',
    sourceHandle: 'bottom',
    type: 'step',
    markerEnd: arrow,
  },
  // {
  //   id: 'friction-brake-failure-braking',
  //   source: 'friction-brake',
  //   target: 'failure-braking',
  //   sourceHandle: 'left',
  //   type: 'step',
  //   markerEnd: arrow,
  // },
  // {
  //   id: 'other-states-failure-braking',
  //   source: 'other-states',
  //   target: 'failure-braking',
  //   targetHandle: 'bottom',
  //   type: 'step',
  //   markerEnd: arrow,
  // },
  {
    id: 'friction-brake-stop-lev',
    source: 'friction-brake',
    target: 'stop-lev',
    sourceHandle: 'right',
    type: 'step',
    markerEnd: arrow,
  },
  //
  {
    id: 'stop-lev-stopped',
    source: 'stop-lev',
    target: 'stopped',
    sourceHandle: 'bottom',
    targetHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'stopped-battery-recharge',
    source: 'stopped',
    target: 'battery-recharge',
    sourceHandle: 'bottom',
    targetHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'battery-recharge-capacitor-discharge',
    source: 'battery-recharge',
    target: 'capacitor-discharge',
    sourceHandle: 'bottom',
    targetHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'capacitor-discharge-safe',
    source: 'capacitor-discharge',
    target: 'safe',
    sourceHandle: 'bottom',
    targetHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'failure-braking-capacitor-discharge',
    source: 'failure-braking',
    target: 'capacitor-discharge',
    sourceHandle: 'bottom',
    targetHandle: 'left',
    type: 'step',
    markerEnd: arrow,
  },
];
