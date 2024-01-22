export type RawLevitationHeight = {
  id: string,
  timestamp: number,
  value: string
}

export type LevitationHeight = {
  id: string;
  timestamp: number;
  value: number;
};

export type LevitationHeightResponse = Record<string, LevitationHeight[]>;
