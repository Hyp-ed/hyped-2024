import ReactFlow, { Background, Position } from 'reactflow';
import 'reactflow/dist/style.css';
import { PodStateType, ALL_POD_STATES } from '@hyped/telemetry-constants';
import { PassiveNode, FailureNode, ActiveNode, NeutralNode } from './nodes';
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
      NeutralNode,
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
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.IDLE,
        },
        position: {
          x: 200,
          y: -100
        },
        type: getNodeType(ALL_POD_STATES.IDLE),
      },
      {
        id: 'calibrate',
        data: {
          label: 'Calibrate',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.CALIBRATE,
        },
        position: {
          x: 200,
          y: 0
        },
        type: getNodeType(ALL_POD_STATES.CALIBRATE),
      },
      {
        id: 'precharge',
        data: {
          label: 'Precharge',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.PRECHARGE,
        },
        position: {
          x: 200,
          y: 100
        },
        type: getNodeType(ALL_POD_STATES.PRECHARGE),
      },
      {
        id: 'ready-lev',
        data: {
          label: 'Ready for Levitation',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.READY_FOR_LEVITATION,
        },
        position: {
          x: 200,
          y: 200,
        },
        type: getNodeType(ALL_POD_STATES.READY_FOR_LEVITATION),
      },
      {
        id: 'begin-lev',
        data: {
          label: 'Begin Levitation',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.BEGIN_LEVITATION,
        },
        position: {
          x: 200,
          y: 300,
        },
        type: getNodeType(ALL_POD_STATES.BEGIN_LEVITATION),
      },
      {
        id: 'ready-launch',
        data: {
          label: 'Ready for Launch',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.READY_FOR_LAUNCH,
        },
        position: {
          x: 200,
          y: 400,
        },
        type: getNodeType(ALL_POD_STATES.READY_FOR_LAUNCH),
      },

      {
        id: 'other-states',
        data: {
          label: 'All other States',
          sourcePositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.TEXT,
        },
        position: {
          x: 500,
          y: 400
        },
        type: getNodeType(ALL_POD_STATES.TEXT),
      },

      {
        id: 'accelerate',
        data: {
          label: 'Accelerate',
          sourcePositions: [
            {
              position: Position.Left,
              id: 'left',
            },
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.ACCELERATE,
        },
        position: {
          x: 500,
          y: -100,
        },
        type: getNodeType(ALL_POD_STATES.ACCELERATE),
      },
      {
        id: 'lim-brake',
        data: {
          label: 'LIM Brake',
          sourcePositions: [
            {
              position: Position.Left,
              id: 'left',
            },
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.LIM_BRAKE,
        },
        position: {
          x: 500,
          y: 0,
        },
        type: getNodeType(ALL_POD_STATES.LIM_BRAKE),
      },
      {
        id: 'friction-brake',
        data: {
          label: 'Friction Brake',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.FRICTION_BRAKE,
        },
        position: {
          x: 500,
          y: 100,
        },
        type: getNodeType(ALL_POD_STATES.FRICTION_BRAKE),
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
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.FAILURE_BRAKING,
        },
        position: {
          x: 500,
          y: 300,
        },
        type: getNodeType(ALL_POD_STATES.FAILURE_BRAKING),
      },

      {
        id: 'stop-lev',
        data: {
          label: 'Stop Levitation',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.STOP_LEVITATION,
        },
        position: {
          x: 800,
          y: 0,
        },
        type: getNodeType(ALL_POD_STATES.STOP_LEVITATION),
      },
      {
        id: 'stopped',
        data: {
          label: 'Stopped',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.STOPPED
        },
        position: {
          x: 800,
          y: 100,
        },
        type: getNodeType(ALL_POD_STATES.STOPPED),
      },
      {
        id: 'battery-recharge',
        data: {
          label: 'Battery Recharge',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.BATTERY_RECHARGE,
        },
        position: {
          x: 800,
          y: 200,
        },
        type: getNodeType(ALL_POD_STATES.BATTERY_RECHARGE),
      },
      {
        id: 'capacitor-discharge',
        data: {
          label: 'Capacitor Discharge',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          active: currentState === ALL_POD_STATES.CAPACITOR_DISCHARGE,
        },
        position: {
          x: 800,
          y: 300,
        },
        type: getNodeType(ALL_POD_STATES.CAPACITOR_DISCHARGE),
      },
      {
        id: 'safe',
        data: {
          label: 'Safe',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
          ],
          active: currentState === ALL_POD_STATES.SAFE,
        },
        position: {
          x: 800,
          y: 400,
        },
        type: getNodeType(ALL_POD_STATES.SAFE),
      },

    ],
    [currentState],
  );

  return (
    <div className="h-full flex flex-col justify-center items-center">
      <div className="h-full w-full">
        <ReactFlow
          nodes={nodes}
          edges={edges}
          nodeTypes={nodeTypes}
          nodesDraggable={false}
          nodesConnectable={false}
          defaultViewport={{
            zoom: 1,
            x: 50,
            y: 250,
          }}
        >
          <Background />
        </ReactFlow>
      </div>
    </div>
  );
}
