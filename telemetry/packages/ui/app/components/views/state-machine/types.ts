import { Node, Position } from 'reactflow';

/**
 * Data passed to custom nodes
 */
export type NodeDataType = {
  label: string;
  sourcePositions?: {
    position: Position;
    id: string;
  }[];
  targetPositions?: {
    position: Position;
    id: string;
  }[];
  active?: boolean;
};

export type CustomNodeType = Omit<Node, 'data'> & {
  data: NodeDataType;
};
