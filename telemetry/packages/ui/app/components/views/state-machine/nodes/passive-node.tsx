import { cn } from '@/lib/utils';
import { memo } from 'react';
import { Handle, NodeProps } from 'reactflow';
import { BASE_NODE_STYLES } from './styles';
import { NodeDataType } from '../types';

const PassiveNode = memo(
  ({
    data,
  }: Omit<NodeProps, 'data'> & {
    data: NodeDataType;
  }) => (
    <>
      {data.targetPositions &&
        data.targetPositions.map(({ position, id }) => (
          <Handle key={id} type="target" position={position} id={id} />
        ))}
      <div
        className={cn(
          BASE_NODE_STYLES,
          data.active
            ? 'border-2 bg-white text-black'
            : 'border-2 text-white border-white border-dashed',
        )}
      >
        {data.label}
      </div>
      {data.sourcePositions &&
        data.sourcePositions.map(({ position, id }) => (
          <Handle key={id} type="source" position={position} id={id} />
        ))}
    </>
  ),
);

PassiveNode.displayName = 'PassiveNode'; // Add display name to the component
export { PassiveNode }; // Export the component
