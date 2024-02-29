import { Prettify } from '../utils/Prettify';

export type PiUnknownVersionStatus = 'unknown';
export type PiKnownVersionStatus = 'up-to-date' | 'out-of-date';
export type PiVersionStatus = PiUnknownVersionStatus | PiKnownVersionStatus;

export type PiStatus = 'online' | 'offline';

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

export type PiInfo = Prettify<
  Pi &
    (
      | (PiVersionResult & {
          versionStatus: PiKnownVersionStatus;
        })
      | {
          versionStatus: PiUnknownVersionStatus;
        }
    ) & {
      podId: string; // TODOLater: This should be a PodId but that's defined in the `constants` package...
      status: PiStatus;
    }
>;
