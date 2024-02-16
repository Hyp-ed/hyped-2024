import {
  HistoricalValueResponse,
  LevitationHeightResponse,
} from '@hyped/telemetry-types';
import {
  DisplacementResponse,
  VelocityResponse,
} from '@hyped/telemetry-types/dist/server/responses';

/**
 * The server endpoint for fetching public data.
 */
export const SERVER_ENDPOINT = `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/${process.env.NEXT_PUBLIC_POD_ID}/public-data`;

const ONE_MINUTE = 60;

/**
 * Gets the last `prev` seconds of displacement data.
 * @param prev The number of seconds to get.
 * @returns The historical displacement data.
 */
export const getDisplacement = async (prev: number = ONE_MINUTE) => {
  const now = new Date().getTime();
  const start = now - prev * 1000;
  const response = await fetch(
    `${SERVER_ENDPOINT}/displacement?start=${start}`,
  );
  return response.json() as Promise<DisplacementResponse>;
};

export const getLevitationHeight = async (prev: number = ONE_MINUTE) => {
  const now = new Date().getTime();
  const start = now - prev * 1000;
  const response = await fetch(
    `${SERVER_ENDPOINT}/levitation-height?start=${start}`,
  );
  return response.json() as Promise<LevitationHeightResponse>;
};

export const getVelocity = async (prev: number = ONE_MINUTE) => {
  const now = new Date().getTime();
  const start = now - prev * 1000;
  const response = await fetch(`${SERVER_ENDPOINT}/velocity?start=${start}`);
  return response.json() as Promise<VelocityResponse>;
};
