import { http } from 'openmct/core/http';
import toast from 'react-hot-toast';

/**
 * Sends a request to the server to update the Pi binary
 * @param podId The pod ID of the Pi
 * @param piId The Pi ID
 * @param branch The branch to update the Pi binary to
 */
export const updatePiBinary = async (
  podId: string,
  piId: string,
  branch: string,
) => {
  await toast.promise(
    http(`pods/${podId}/pis/${piId}/update-binary?compareBranch=${branch}`, {
      method: 'POST',
    }),
    {
      loading: 'Updating Pi binary...',
      success: 'Pi binary updated!',
      error: 'Failed to update Pi binary',
    },
  );
};

/**
 * Sends a request to the server to update the Pi config
 * @param podId The pod ID of the Pi
 * @param piId The Pi ID
 * @param branch The branch to update the Pi config to
 */
export const updatePiConfig = async (
  podId: string,
  piId: string,
  branch: string,
) => {
  await toast.promise(
    http(`pods/${podId}/pis/${piId}/update-config?compareBranch=${branch}`, {
      method: 'POST',
    }),
    {
      loading: 'Updating Pi config...',
      success: 'Pi config updated!',
      error: 'Failed to update Pi config',
    },
  );
};
