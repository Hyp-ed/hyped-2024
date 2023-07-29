import ReactFlow, { Position } from 'reactflow';
import 'reactflow/dist/style.css';
import { PodStateType, ALL_POD_STATES } from '@hyped/telemetry-constants';
import { PassiveNode, FailureNode, ActiveNode, TextNode } from './nodes';
import { useMemo } from 'react';
import './styles.css';
import { getNodeType } from './utils';
import { edges } from './edges';
import { CustomNodeType } from './types';

export function StateMachineFlowChart({
  currentState,
}: {
  currentState: PodStateType;
}) {
  const nodeTypes = useMemo(
    () => ({
      FailureNode,
      PassiveNode,
      ActiveNode,
      TextNode,
    }),
    [],
  );

  const nodes: CustomNodeType[] = useMemo(
    () => [
      {
        id: 'idle',
        data: {
          label: 'Idle',
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          active: currentState === ALL_POD_STATES.IDLE,
        },
        position: { x: 0, y: 200 },
        type: getNodeType(ALL_POD_STATES.IDLE),
      },
      {
        id: 'calibrating',
        data: {
          label: 'Calibrating',
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
            },
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          active: currentState === ALL_POD_STATES.CALIBRATING,
        },
        position: { x: 200, y: 200 },
        type: getNodeType(ALL_POD_STATES.CALIBRATING),
      },
      {
        id: 'failure-calibrating',
        data: {
          label: 'Failure Calibrating',
          targetPositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.FAILURE_CALIBRATING,
        },
        position: { x: 200, y: 0 },
        type: getNodeType(ALL_POD_STATES.FAILURE_CALIBRATING),
      },
      {
        id: 'ready',
        data: {
          label: 'Ready',
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          active: currentState === ALL_POD_STATES.READY,
        },
        position: {
          x: 400,
          y: 200,
        },
        type: getNodeType(ALL_POD_STATES.READY),
      },
      {
        id: 'accelerating',
        data: {
          label: 'Accelerating',
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
            },
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.ACCELERATING,
        },
        position: {
          x: 600,
          y: 200,
        },
        type: getNodeType(ALL_POD_STATES.ACCELERATING),
      },
      {
        id: 'nominal-braking',
        data: {
          label: 'Nominal Braking',
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
            },
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.NOMINAL_BRAKING,
        },
        position: {
          x: 800,
          y: 200,
        },
        type: getNodeType(ALL_POD_STATES.NOMINAL_BRAKING),
      },
      {
        id: 'stopped',
        data: {
          label: 'Stopped',
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          active: currentState === ALL_POD_STATES.STOPPED,
        },
        position: {
          x: 1000,
          y: 200,
        },
        type: getNodeType(ALL_POD_STATES.STOPPED),
      },
      {
        id: 'off',
        data: {
          label: 'Off',
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.OFF,
        },
        position: {
          x: 1200,
          y: 200,
        },
        type: getNodeType(ALL_POD_STATES.OFF),
      },
      {
        id: 'failure-braking',
        data: {
          label: 'Failure Braking',
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          active: currentState === ALL_POD_STATES.FAILURE_BRAKING,
        },
        position: {
          x: 1000,
          y: 0,
        },
        type: getNodeType(ALL_POD_STATES.FAILURE_BRAKING),
      },
      {
        id: 'failure-stopped',
        data: {
          label: 'Failure Stopped',
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.FAILURE_STOPPED,
        },
        position: {
          x: 1200,
          y: 0,
        },
        type: getNodeType(ALL_POD_STATES.FAILURE_STOPPED),
      },
      {
        id: 'key-label',
        data: {
          label: 'Key:',
        },
        position: {
          x: 0,
          y: 360,
        },
        type: 'TextNode',
      },
      {
        id: 'key-passive',
        data: {
          label: 'Passive State',
        },
        position: {
          x: 0,
          y: 400,
        },
        type: 'PassiveNode',
      },
      {
        id: 'key-active',
        data: {
          label: 'Active State',
        },
        position: {
          x: 175,
          y: 400,
        },
        type: 'ActiveNode',
      },
      {
        id: 'key-failure',
        data: {
          label: 'Failure State',
        },
        position: {
          x: 350,
          y: 400,
        },
        type: 'FailureNode',
      },
    ],
    [currentState],
  );

  return (
    <div className="min-h-[500px] pt-8">
      <ReactFlow
        nodes={nodes}
        edges={edges}
        nodeTypes={nodeTypes}
        // remove all interactivity
        panOnDrag={false}
        panOnScroll={false}
        zoomOnScroll={false}
        elementsSelectable={false}
        nodesDraggable={false}
        nodesConnectable={false}
        zoomOnPinch={false}
        zoomOnDoubleClick={false}
      />
    </div>
  );
}
//
