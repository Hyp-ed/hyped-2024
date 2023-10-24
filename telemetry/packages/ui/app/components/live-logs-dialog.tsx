import { LogViewer } from './log-viewer';
import { Button } from './ui/button';
import {
  Dialog,
  DialogContent,
  DialogFooter,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from '@/components/ui/dialog';

export const LiveLogsDialog = () => {
  return (
    <Dialog>
      <DialogTrigger asChild>
        <Button>Open Live Logs</Button>
      </DialogTrigger>
      <DialogContent className="min-w-[85%]">
        <DialogHeader>
          <DialogTitle>Live Server Logs</DialogTitle>
        </DialogHeader>
        <LogViewer />
        <DialogFooter>
          <DialogTrigger asChild>
            <Button>Close</Button>
          </DialogTrigger>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
};
