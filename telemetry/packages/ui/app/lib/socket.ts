import { io } from 'socket.io-client';

// TODO: Move to env
const SERVER_ENDPOINT = 'http://localhost:3000';
const PATH = '/live-logs';

export const socket = io(SERVER_ENDPOINT, {
  path: PATH,
});
