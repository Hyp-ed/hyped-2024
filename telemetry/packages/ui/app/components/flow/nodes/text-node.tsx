import { memo } from 'react';
import { NodeProps } from 'reactflow';
import { NodeDataType } from '../types';

export const TextNode = memo(function TextNode({
  data,
}: Omit<NodeProps, 'data'> & {
  data: NodeDataType;
}) {
  return (
    <>
      <p>{data.label}</p>
    </>
  );
});
