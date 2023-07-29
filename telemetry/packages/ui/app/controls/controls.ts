import { log } from '@/lib/logger';
import { http } from 'openmct/core/http';
import { toast } from 'react-hot-toast';

export const startPod = async (podId: string) => {
  toast.success(`[${podId}] Pod launched!`, { icon: '🚀' });
  const res = await http.post(`pods/${podId}/controls/start`);
  return res.status === 200;
};

export const stopPod = async (podId: string) => {
  toast(`[${podId}] Pod stopped!`, { icon: '🛑' });
  log(`UI button clicked: stop`, podId);
  const res = await http.post(`pods/${podId}/controls/stop`);
  return res.status === 200;
};

export const clamp = async (podId: string) => {
  log(`UI button clicked: clamped`, podId);
  const res = await http.post(`pods/${podId}/controls/clamp`);
  return res.status === 200;
};

export const retract = async (podId: string) => {
  toast(`[${podId}] Retracted!`);
  log(`UI button clicked: retracted`, podId);
  const res = await http.post(`pods/${podId}/controls/retract`);
  return res.status === 200;
};

export const raise = async (podId: string) => {
  toast(`[${podId}] Raised!`);
  log(`UI button clicked: raised`, podId);
  const res = await http.post(`pods/${podId}/controls/raise`);
  return res.status === 200;
};

export const lower = async (podId: string) => {
  toast(`[${podId}] Lowered!`);
  log(`UI button clicked: lowered`, podId);
  const res = await http.post(`pods/${podId}/controls/lower`);
  return res.status === 200;
};

export const startHP = async (podId: string) => {
  toast(`[${podId}] HP started!`);
  log(`UI button clicked: start-hp`, podId);
  const res = await http.post(`pods/${podId}/controls/start-hp`);
  return res.status === 200;
};

export const stopHP = async (podId: string) => {
  toast(`[${podId}] HP stopped!`);
  log(`UI button clicked: stop-hp`, podId);
  const res = await http.post(`pods/${podId}/controls/stop-hp`);
  return res.status === 200;
};

export const tilt = async (podId: string) => {
  toast(`[${podId}] tilt!`);
  const res = await http.post(`pods/${podId}/controls/tilt`);
  return res.status === 200;
};
