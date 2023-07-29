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
    id: 'idle-calibrating',
    source: 'idle',
    target: 'calibrating',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'calibrating-ready',
    source: 'calibrating',
    target: 'ready',
    sourceHandle: 'right',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'calibrating-failure-calibrating',
    source: 'calibrating',
    target: 'failure-calibrating',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'ready-accelerating',
    source: 'ready',
    target: 'accelerating',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'accelerating-nominal-braking',
    source: 'accelerating',
    target: 'nominal-braking',
    type: 'step',
    markerEnd: arrow,
  },
  {
    id: 'accelerating-failure-braking',
    source: 'accelerating',
    target: 'failure-braking',
    sourceHandle: 'top',
    type: 'step',
    markerEnd: arrow,
  },
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
