import { memo } from 'react';
import { Handle, NodeProps } from 'reactflow';
import { BASE_NODE_STYLES } from './styles';
import { cn } from '@/lib/utils';
import { NodeDataType } from '../types';

export const ActiveNode = memo(
  ({
    data,
  }: Omit<NodeProps, 'data'> & {
    data: NodeDataType;
  }) => (
    <>
      {data.targetPositions &&
        data.targetPositions.map(({ position, id }) => (
          <Handle type="target" position={position} id={id} />
        ))}
      <div
        className={cn(
          BASE_NODE_STYLES,
          data.active
            ? 'border-2 border-green-200 bg-green-700 text-green-200'
            : 'border-2 border-green-600 text-white border-dashed',
        )}
      >
        {' '}
        {data.label}
      </div>
      {data.sourcePositions &&
        data.sourcePositions.map(({ position, id }) => (
          <Handle type="source" position={position} id={id} />
        ))}
    </>
  ),
);
