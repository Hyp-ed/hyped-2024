import { Column, ColumnDef } from '@tanstack/react-table';
import { ActionsMenu } from './actions-menu';
import { ArrowDown, ArrowUp, ArrowUpDown } from 'lucide-react';
import { Button } from '../../ui/button';
import { Checkbox } from '@/components/ui/checkbox';
import { PiInfo } from '@hyped/telemetry-types';

const UNKNOWN = 'unknown';

export const columns = (compareBranch: string): ColumnDef<PiInfo>[] => [
  {
    id: 'select',
    header: ({ table }) => (
      <Checkbox
        checked={
          table.getIsAllPageRowsSelected() ||
          (table.getIsSomePageRowsSelected() && 'indeterminate')
        }
        onCheckedChange={(value) => table.toggleAllPageRowsSelected(!!value)}
        aria-label="Select all"
      />
    ),
    cell: ({ row }) => (
      <Checkbox
        checked={row.getIsSelected()}
        onCheckedChange={(value) => row.toggleSelected(!!value)}
        aria-label="Select row"
      />
    ),
    enableSorting: false,
    enableHiding: false,
  },
  {
    accessorKey: 'id',
    header: ({ column }) => <SortableHeader column={column}>ID</SortableHeader>,
  },
  {
    accessorKey: 'hostname',
    header: ({ column }) => (
      <SortableHeader column={column}>Hostname</SortableHeader>
    ),
  },
  {
    accessorKey: 'ip',
    header: 'IP Address',
  },
  {
    accessorKey: 'status',
    header: ({ column }) => (
      <SortableHeader column={column}>Connection Status</SortableHeader>
    ),
    cell: ({ row }) => row.getValue('status') || UNKNOWN,
  },
  {
    accessorKey: 'binaryHash',
    header: ({ column }) => (
      <SortableHeader column={column}>Binary Hash</SortableHeader>
    ),
    cell: ({ row }) => row.getValue('binaryHash') || UNKNOWN,
  },
  {
    accessorKey: 'binaryStatus',
    header: ({ column }) => (
      <SortableHeader column={column}>Binary Status</SortableHeader>
    ),
    cell: ({ row }) => row.getValue('binaryStatus') || UNKNOWN,
  },
  {
    accessorKey: 'configHash',
    header: ({ column }) => (
      <SortableHeader column={column}>Config Hash</SortableHeader>
    ),
    cell: ({ row }) => row.getValue('configHash') || UNKNOWN,
  },
  {
    accessorKey: 'configStatus',
    header: ({ column }) => (
      <SortableHeader column={column}>Config Status</SortableHeader>
    ),
    cell: ({ row }) => row.getValue('configStatus') || UNKNOWN,
  },
  {
    id: 'actions',
    cell: ({ row }) => <ActionsMenu row={row} compareBranch={compareBranch} />,
  },
];

const SortableHeader = ({
  column,
  children,
}: {
  column: Column<PiInfo, unknown>;
  children: React.ReactNode;
}) => {
  return (
    <Button
      variant="link"
      className="flex gap-1 text-muted-foreground px-0"
      onClick={() => column.toggleSorting(column.getIsSorted() === 'asc')}
    >
      {children}
      {column.getIsSorted() === 'asc' ? (
        <ArrowUp size={16} />
      ) : column.getIsSorted() === 'desc' ? (
        <ArrowDown size={16} />
      ) : (
        <ArrowUpDown size={16} />
      )}
    </Button>
  );
};
