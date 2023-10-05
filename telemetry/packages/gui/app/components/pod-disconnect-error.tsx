import {
  AlertDialog,
  AlertDialogAction,
  AlertDialogContent,
  AlertDialogDescription,
  AlertDialogFooter,
  AlertDialogHeader,
  AlertDialogTitle,
} from '@/components/ui/alert-dialog';
import { useEffect, useState } from 'react';
import {
  POD_CONNECTION_STATUS,
  PodConnectionStatusType,
} from '@/types/PodConnectionStatus';
import { log } from '@/lib/logger';

export const PodDisconnectError = ({
  podId,
  status,
}: {
  podId: string;
  status: PodConnectionStatusType;
}) => {
  const [open, setOpen] = useState(false);

  // Open dialog when pod disconnects or encounters an error
  useEffect(() => {
    const disconnected =
      status === POD_CONNECTION_STATUS.DISCONNECTED ||
      status === POD_CONNECTION_STATUS.ERROR;
    setOpen(disconnected);
    if (disconnected) log('Pod disconnected!', podId);
  }, [status]);

  const close = () => {
    setOpen(false);
    log('Pod disconnect dialog acknowledged', podId);
  };

  return (
    <AlertDialog open={open}>
      <AlertDialogContent>
        <AlertDialogHeader>
          <AlertDialogTitle>Pod disconnected!</AlertDialogTitle>
          <AlertDialogDescription>
            Lost connection to pod.
          </AlertDialogDescription>
        </AlertDialogHeader>
        <AlertDialogFooter>
          <AlertDialogAction
            className="bg-red-700 hover:bg-red-800 text-white"
            onClick={close}
          >
            Okay
          </AlertDialogAction>
        </AlertDialogFooter>
      </AlertDialogContent>
    </AlertDialog>
  );
};
