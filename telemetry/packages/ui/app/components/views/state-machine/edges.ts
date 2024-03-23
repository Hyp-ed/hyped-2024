import { Edge, EdgeMarkerType, MarkerType } from 'reactflow';
import { MODES, MODE_INACTIVE_STATES, ModeType, PodStateType } from '@hyped/telemetry-constants';
import { CustomEdgeType } from './types';

// defines the arrow marker for the edges
export const arrow: EdgeMarkerType = {
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
    'ready-for-levitation',
    'begin-levitation',
    'ready-for-launch',
  ],
  center: ['accelerate', 'lim-brake', 'friction-brake'],
  right: [
    'stop-levitation',
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

export const edges: CustomEdgeType[] = [
  {
    id: 'idle-calibrate',
    source: 'idle',
    target: 'calibrate',
    sourceHandle: 'top',
    targetHandle: 'bottom',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'calibrate-precharge',
    source: 'calibrate',
    target: 'precharge',
    sourceHandle: 'top',
    targetHandle: 'bottom',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'precharge-ready-for-levitation',
    source: 'precharge',
    target: 'ready-for-levitation',
    sourceHandle: 'top',
    targetHandle: 'bottom',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    // TODO: Add a dynamic connection
    // Write a function which takes the mode
    // and returns string for source/target/handle properties
    
    /*
    const getEdgeConnection = (mode: string) => {
      const alterations = (() => {
      switch (mode) {
        case 'LEV...':
        return {
          sourceHandle: 'right',
          target: 'stop-levitation',
          targetHandle: 'left',
        };
        // Add more cases for different modes
        default:
        return {};
      }
      })();

      return {
      source: 'ready-for-levitation',
      target: 'begin-levitation',
      sourceHandle: 'top',
      type: 'smoothstep',
            pathOptions: {
        borderRadius: 20,
      },
      markerEnd: arrow,
      ...alterations,
      };
    };
    */

    // sourceHandle: right when lev-only
    id: 'ready-for-levitation-begin-levitation',
    source: 'ready-for-levitation',
    target: 'begin-levitation',
    sourceHandle: 'top-right',
    targetHandle: 'bottom-left',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    // target: stop-levitation when lev-only
    // sourceHandle: right when lev-only
    // targetHandle: left when lev-only
    id: 'begin-levitation-ready-for-launch',
    source: 'begin-levitation',
    target: 'ready-for-launch',
    sourceHandle: 'top-right',
    targetHandle: 'bottom-left',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'ready-for-launch-accelerate',
    source: 'ready-for-launch',
    target: 'accelerate',
    sourceHandle: 'right-top',
    targetHandle: 'top-top',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'accelerate-lim-brake',
    source: 'accelerate',
    target: 'lim-brake',
    sourceHandle: 'bottom-bottom',
    targetHandle: 'top-top',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'lim-brake-friction-brake',
    source: 'lim-brake',
    target: 'friction-brake',
    sourceHandle: 'bottom-bottom',
    targetHandle: 'top-top',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'friction-brake-stop-levitation',
    source: 'friction-brake',
    target: 'stop-levitation',
    sourceHandle: 'right',
    targetHandle: 'left',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  //
  {
    id: 'stop-levitation-stopped',
    source: 'stop-levitation',
    target: 'stopped',
    sourceHandle: 'bottom',
    targetHandle: 'top',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'stopped-battery-recharge',
    source: 'stopped',
    target: 'battery-recharge',
    sourceHandle: 'bottom',
    targetHandle: 'top',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'battery-recharge-capacitor-discharge',
    source: 'battery-recharge',
    target: 'capacitor-discharge',
    sourceHandle: 'bottom',
    targetHandle: 'top',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'capacitor-discharge-safe',
    source: 'capacitor-discharge',
    target: 'safe',
    sourceHandle: 'bottom',
    targetHandle: 'top',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
  {
    id: 'failure-braking-capacitor-discharge',
    source: 'failure-braking',
    target: 'capacitor-discharge',
    sourceHandle: 'right',
    targetHandle: 'left',
    type: 'smoothstep',
    pathOptions: {
      borderRadius: 20,
    },
    markerEnd: arrow,
  },
];


export const writeEdges = (mode: ModeType): CustomEdgeType[] => {

  // return edges;

  return edges.filter((edge) => {
    return !MODE_INACTIVE_STATES[mode].includes(
      edge.source.replace(/-/g, '_').toUpperCase() as PodStateType)
  })
  .map((edge, i, arr) => {
    if (i < arr.length - 2) {
      const regex = new RegExp(`${arr[i + 1].source}$`)
      if ( !regex.test( edge.id ) ) {
        edge.id = `${edge.source}-${arr[i + 1].source}`;
        edge.target = arr[i + 1].source;
      }
    }
    return {
      ...edge,
      // id: `${edge.source}-${arr[i + 1].source}`,
      // target: arr[i + 1].source,
      sourceHandle: edge.sourceHandle.includes('-')
      ? (mode === 'ALL_SYSTEMS_ON' 
        ? edge.sourceHandle.split('-')[0] : edge.sourceHandle.split('-')[1])
      : edge.sourceHandle,
      targetHandle: edge.targetHandle.includes('-')
      ? (mode === 'ALL_SYSTEMS_ON' 
        ? edge.targetHandle.split('-')[0] : edge.targetHandle.split('-')[1])
      : edge.targetHandle,
    }
  })

}
