import { LevitationHeightResponse } from '@hyped/telemetry-types';
import {
  DisplacementResponse,
  LaunchTimeResponse,
  VelocityResponse,
} from '@hyped/telemetry-types/dist/server/responses';

/**
 * The server endpoint for fetching public data.
 */
export const SERVER_ENDPOINT = `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/${process.env.NEXT_PUBLIC_POD_ID}/public-data`;

const ONE_MINUTE = 60;

/**
 * Gets the last `prev` seconds of displacement data.
 * Should return a promise that resolves to the historical displacement data,
 * or rejects if the request fails.
 * @param prev The number of seconds to get.
 * @returns The historical displacement data.
 */
export const getDisplacement = async (
  prev: number = ONE_MINUTE,
): Promise<DisplacementResponse> => {
  const now = new Date().getTime();
  const start = now - prev * 1000;
  const response = await fetch(
    `${SERVER_ENDPOINT}/displacement?start=${start}`,
  ).then((res) => res.json());
  if (response.statusCode !== 200)
    throw new Error('Failed to fetch displacement');
  return response as Promise<DisplacementResponse>;
};

export const getLevitationHeight = async (
  prev: number = ONE_MINUTE,
): Promise<LevitationHeightResponse> => {
  const now = new Date().getTime();
  const start = now - prev * 1000;
  const response = await fetch(
    `${SERVER_ENDPOINT}/levitation-height?start=${start}`,
  ).then((res) => res.json());
  if (response.statusCode !== 200)
    throw new Error('Failed to fetch levitation height');
  return response as Promise<LevitationHeightResponse>;
};

export const getVelocity = async (
  prev: number = ONE_MINUTE,
): Promise<VelocityResponse> => {
  const now = new Date().getTime();
  const start = now - prev * 1000;
  const response = await fetch(
    `${SERVER_ENDPOINT}/velocity?start=${start}`,
  ).then((res) => res.json());
  if (response.statusCode !== 200) throw new Error('Failed to fetch velocity');
  return response as Promise<VelocityResponse>;
};

export const getLaunchTime = async (
  prev: number = ONE_MINUTE,
): Promise<LaunchTimeResponse> => {
  const now = new Date().getTime();
  const start = now - prev * 1000;
  const response = await fetch(
    `${SERVER_ENDPOINT}/launch-time?start=${start}`,
  ).then((res) => res.json());
  if (response.statusCode !== 200)
    throw new Error('Failed to fetch launch time');
  return response.json() as Promise<LaunchTimeResponse>;
};
