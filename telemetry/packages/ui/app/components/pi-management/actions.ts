import { http } from 'openmct/core/http';
import toast from 'react-hot-toast';

export const updatePiBinary = async (podId: string, piId: string) => {
  await toast.promise(
    http(`pods/${podId}/pis/${piId}/update-binary`, {
      method: 'POST',
    }),
    {
      loading: 'Updating Pi binary...',
      success: 'Pi binary updated!',
      error: 'Failed to update Pi binary',
    },
  );
};

export const updatePiConfig = async (podId: string, piId: string) => {
  await toast.promise(
    http(`pods/${podId}/pis/${piId}/update-config`, {
      method: 'POST',
    }),
    {
      loading: 'Updating Pi config...',
      success: 'Pi config updated!',
      error: 'Failed to update Pi config',
    },
  );
};
