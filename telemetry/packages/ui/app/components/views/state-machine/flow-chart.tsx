import ReactFlow, { Background, Position, Edge } from 'reactflow';
import 'reactflow/dist/style.css';
import {
  PodStateType,
  ALL_POD_STATES,
  MODE_INACTIVE_STATES,
} from '@hyped/telemetry-constants';
import { PassiveNode, FailureNode, ActiveNode, NeutralNode } from './nodes';
import { useMemo, useEffect, useState } from 'react';
import './styles.css';
import { getNodeType } from './utils';
import { writeEdges, getEdgeType, arrow } from './edges';
import { CustomEdgeType, CustomNodeType } from './types';
import { useCurrentMode } from '@/context/pods';


export function StateMachineFlowChart({
  currentState,
  onModeChange: onModeChange
}: {
  currentState: PodStateType;
  onModeChange: any;
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

  const { currentMode } = useCurrentMode();
  
  const [displayNodes, setDisplayNodes]: [CustomNodeType[], any] = useState([])
  const [edges, setEdges] = useState(writeEdges(currentMode));
  const [failNode, setFailNode]: [CustomEdgeType, any] = useState(edges[0]);
  


  const nodes: CustomNodeType[] = useMemo(
    () => [
      {
        id: 'idle',
        data: {
          label: 'Idle',
          sourcePositions: [
            {
              position: Position.Top,
              id: 'top',
            },
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          active: currentState === ALL_POD_STATES.IDLE,
        },
        position: {
          x: 200,
          y: 400,
        },
        type: getNodeType(ALL_POD_STATES.IDLE),
      },
      {
        id: 'calibrate',
        data: {
          label: 'Calibrate',
          sourcePositions: [
            {
              position: Position.Top,
              id: 'top',
            },
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          targetPositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.CALIBRATE,
        },
        position: {
          x: 200,
          y: 300,
        },
        type: getNodeType(ALL_POD_STATES.CALIBRATE),
      },
      {
        id: 'precharge',
        data: {
          label: 'Precharge',
          sourcePositions: [
            {
              position: Position.Top,
              id: 'top',
            },
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          targetPositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.PRECHARGE,
        },
        position: {
          x: 200,
          y: 200,
        },
        type: getNodeType(ALL_POD_STATES.PRECHARGE),
      },
      {
        id: 'ready-for-levitation',
        data: {
          label: 'Ready for Levitation',
          sourcePositions: [
            // {
            //   position: Position.Top,
            //   id: 'top',
            // },
            ...(currentMode == 'ALL_SYSTEMS_ON' 
              ? [{ position: Position.Top, id: 'top' }] 
              : []),
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          targetPositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.READY_FOR_LEVITATION,
        },
        position: {
          x: 200,
          y: 100,
        },
        type: getNodeType(ALL_POD_STATES.READY_FOR_LEVITATION),
      },
      {
        id: 'begin-levitation',
        data: {
          label: 'Begin Levitation',
          sourcePositions: [
            ...(currentMode == 'ALL_SYSTEMS_ON' 
              ? [{ position: Position.Top, id: 'top' }] 
              : []),
            {
              position: Position.Right,
              id: 'right',
            },
            // ...(currentMode == 'ALL_SYSTEMS_ON' 
            //   ? [{ position: Position.Top, id: 'top' }] 
            //   : []),
          ],
          targetPositions: [
            {
              position: currentMode == 'LEVITATION_ONLY' ? Position.Left : Position.Bottom,
              id: currentMode == 'LEVITATION_ONLY' ? 'left' :  'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.BEGIN_LEVITATION,
        },
        position: currentMode == 'LEVITATION_ONLY' ? {
          x: 500,
          y: 100,
        } : {
          x: 200,
          y: 0, 
        },
        type: getNodeType(ALL_POD_STATES.BEGIN_LEVITATION),
      },
      {
        id: 'ready-for-launch',
        data: {
          label: 'Ready for Launch',
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
            },
            ...(currentMode == 'LIM_ONLY' 
              ? [{ position: Position.Top, id: 'top' }] 
              : []),
          ],
          targetPositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
          ],
          active: currentState === ALL_POD_STATES.READY_FOR_LAUNCH,
        },
        position: currentMode == 'LIM_ONLY' ? {
          x: 200,
          y: 100,
        } : {
          x: 200,
          y: -100
        },
        type: getNodeType(ALL_POD_STATES.READY_FOR_LAUNCH),
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
              position: Position.Left,
              id: 'left',
            },
            {
              position: Position.Top,
              id: 'top',
            },
            // ...(currentMode != 'LIM_ONLY' 
            //   ? [{ position: Position.Top, id: 'top' }] 
            //   : []),
          ],
          active: currentState === ALL_POD_STATES.ACCELERATE,
        },
        position: currentMode == 'LIM_ONLY' ? {
          x: 500,
          y: 100,
        } : {
          x: 500,
          y: 0
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
        position: currentMode == 'LIM_ONLY' ? {
          x: 500,
          y: 200,
        } : {
          x: 500,
          y: 100
        },
        type: getNodeType(ALL_POD_STATES.LIM_BRAKE),
      },
      {
        id: 'friction-brake',
        data: {
          label: 'Friction Brake',
          sourcePositions: [
            {
              position: Position.Right,
              id: 'right',
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
        position: currentMode == 'LIM_ONLY' ? {
          x: 500,
          y: 300,
        } : {
          x: 500,
          y: 200
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
              position: Position.Right,
              id: 'right',
            },
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
        position: currentMode != 'LIM_ONLY' ? {
          x: 500,
          y: 300,
        } : {
          x: 500,
          y: 400
        },
        type: getNodeType(ALL_POD_STATES.FAILURE_BRAKING),
      },

      {
        id: 'stop-levitation',
        data: {
          label: 'Stop Levitation',
          sourcePositions: [
            {
              position: Position.Bottom,
              id: 'bottom',
            },
            {
              position: Position.Right,
              id: 'right',
            }
            // ...(currentState == ALL_POD_STATES.STOP_LEVITATION
            //   ? [{
            //     position: Position.Right,
            //     id: 'right',
            //   }]
            //   : []),
          ],
          targetPositions: [
            {
              position: Position.Left,
              id: 'left',
            },
          ],
          active: currentState === ALL_POD_STATES.STOP_LEVITATION,
        },
        position: {
          x: 800,
          y: currentMode == 'LEVITATION_ONLY' ? 100 : 0,
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
            {
              position: Position.Right,
              id: 'right',
            },
          ],
          targetPositions: [
            {
              position: Position.Top,
              id: 'top',
            },
            { // only for lim-only
              position: Position.Left,
              id: 'left',
            },
          ],
          active: currentState === ALL_POD_STATES.STOPPED,
        },
        position: {
          x: 800,
          y: currentMode == 'LEVITATION_ONLY' ? 200 : 100,
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
            {
              position: Position.Right,
              id: 'right',
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
            {
              position: Position.Right,
              id: 'right',
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
          active: currentState === ALL_POD_STATES.SAFE,
        },
        position: {
          x: 800,
          y: 400,
        },
        type: getNodeType(ALL_POD_STATES.SAFE),
      },
    ],
    [currentState, currentMode],
  );


  useEffect(() => {
    const active = nodes.find((n) => n.data.active) as CustomNodeType;
    setFailNode({
      id: `${active.id}-failure-braking`,
      source: active.id,
      target: 'failure-braking',
      sourceHandle: getEdgeType(active.id)[0],
      targetHandle: getEdgeType(active.id)[1],
      type: 'smoothstep',
      pathOptions: {
        borderRadius: 20,
      },
      markerEnd: arrow,
    });
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [currentState, currentMode, edges]);
  

  useEffect(() => {
    setDisplayNodes(nodes.filter(
      node => !MODE_INACTIVE_STATES[currentMode].includes(
        node.id.replace(/-/g, '_').toUpperCase() as PodStateType
      )));
  // eslint-disable-next-line react-hooks/exhaustive-deps
}, [currentState, currentMode])

  useEffect(() => {
    setEdges([...writeEdges(currentMode), failNode]);
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [displayNodes, currentState, failNode])

  useEffect(() => {
    onModeChange(currentMode);
  })

  return (
    <div className="h-full flex flex-col justify-center items-center">
      <div className="h-full w-full">
        <ReactFlow
          nodes={displayNodes}
          // nodes={
          edges={edges}
          // edges={[failNode].concat( getEdges(currentMode) )}
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
