import {
  Binary,
  Copy,
  HardDriveUpload,
  MoreHorizontal,
  Settings,
  Terminal,
} from 'lucide-react';
import { Button } from '@/components/ui/button';
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuLabel,
  DropdownMenuSeparator,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu';
import { Row } from '@tanstack/react-table';
import { PiInfo } from '@hyped/telemetry-types';
import { updatePiBinary, updatePiConfig } from './actions';
import { useQueryClient } from 'react-query';

export const ActionsMenu = ({ row }: { row: Row<PiInfo> }) => {
  const pi = row.original;

  const queryClient = useQueryClient();
  const refresh = () => queryClient.invalidateQueries('pis');

  return (
    <>
      <DropdownMenu>
        <DropdownMenuTrigger asChild>
          <Button variant="ghost" className="h-8 w-8 p-0">
            <span className="sr-only">Open menu</span>
            <MoreHorizontal className="h-4 w-4" />
          </Button>
        </DropdownMenuTrigger>
        <DropdownMenuContent align="end">
          <DropdownMenuLabel>Actions</DropdownMenuLabel>
          <DropdownMenuItem
            className="flex gap-2"
            onClick={() => void navigator.clipboard.writeText(pi.ip)}
          >
            <Copy size={18} />
            Copy IP address
          </DropdownMenuItem>
          {/* TODO: implement */}
          <DropdownMenuItem className="flex gap-2">
            <Terminal size={18} />
            SSH into Pi
          </DropdownMenuItem>
          <DropdownMenuSeparator />
          <DropdownMenuLabel>Update</DropdownMenuLabel>
          <DropdownMenuItem
            className="flex gap-2"
            onClick={() =>
              void updatePiBinary(pi.podId, pi.id).then(() => refresh())
            }
          >
            <Binary size={18} />
            Update Binary
          </DropdownMenuItem>
          <DropdownMenuItem
            className="flex gap-2"
            onClick={() =>
              void updatePiConfig(pi.podId, pi.id).then(() => refresh())
            }
          >
            <Settings size={18} />
            Update Config
          </DropdownMenuItem>
          <DropdownMenuItem
            className="flex gap-2"
            onClick={() => {
              void Promise.all([
                updatePiBinary(pi.podId, pi.id),
                updatePiConfig(pi.podId, pi.id),
              ]).then(() => refresh());
            }}
          >
            <HardDriveUpload size={18} />
            Update Both
          </DropdownMenuItem>
        </DropdownMenuContent>
      </DropdownMenu>
    </>
  );
};
