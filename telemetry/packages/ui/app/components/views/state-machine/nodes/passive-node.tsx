import { Handle, NodeProps } from 'reactflow';
import { BASE_NODE_STYLES } from './styles';
import { memo } from 'react';
import { cn } from '@/lib/utils';
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
            ? 'border-2 border-sky-200 bg-sky-700 text-sky-200'
            : 'border-2 border-sky-600 text-white border-dashed',
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
