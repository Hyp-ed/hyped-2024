import { Prettify } from '../utils/Prettify';

export type PiUnknownStatus = 'unknown';
export type PiKnownStatus = 'up-to-date' | 'out-of-date';
export type PiStatus = PiUnknownStatus | PiKnownStatus;

// Could replace with stricter type in future
type IpAddress = string;

export type PiId = string;

export type Pi = {
  id: PiId;
  ip: IpAddress;
  name: string;
};

export type PiVersionResult = {
  binaryHash: string;
  configHash: string;
};

export type PiWithVersion = Prettify<
  Pi &
    (
      | (PiVersionResult & {
          status: PiKnownStatus;
        })
      | {
          status: PiUnknownStatus;
        }
    )
>;
