import { memo } from 'react';
import { NodeProps } from 'reactflow';
import { NodeDataType } from '../types';

export const TextNode = memo(
  ({
    data,
  }: Omit<NodeProps, 'data'> & {
    data: NodeDataType;
  }) => (
    <>
      <p>{data.label}</p>
    </>
  ),
);
