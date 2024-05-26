import { Prettify } from '../utils/Prettify';

export type VersionStatus = 'unknown' | 'up-to-date' | 'out-of-date';
export type PiConnectionStatus = 'online' | 'offline';

// Could replace with stricter type in future
type IpAddress = string;

export type PiId = string;

export type Pi = {
  id: PiId;
  ip: IpAddress;
  hostname: string;
};

type Hash = string | null;

export type Hashes = {
  binaryHash: Hash;
  configHash: Hash;
};

export type VersionStatuses = {
  binaryStatus: VersionStatus;
  configStatus: VersionStatus;
};

export type PiInfo = Prettify<
  Pi &
    Hashes &
    VersionStatuses & {
      podId: string;
      connectionStatus: PiConnectionStatus;
    }
>;
