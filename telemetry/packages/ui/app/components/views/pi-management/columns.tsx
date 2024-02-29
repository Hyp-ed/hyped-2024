import { Column, ColumnDef } from '@tanstack/react-table';
import { ActionsMenu } from './actions-menu';
import { ArrowDown, ArrowUp, ArrowUpDown } from 'lucide-react';
import { Button } from '../../ui/button';
import { Checkbox } from '@/components/ui/checkbox';
import { PiInfo } from '@hyped/telemetry-types';

const UNKNOWN = 'unknown';

export const columns: ColumnDef<PiInfo>[] = [
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
    accessorKey: 'name',
    header: ({ column }) => (
      <SortableHeader column={column}>Name</SortableHeader>
    ),
  },
  {
    accessorKey: 'ip',
    header: 'IP Address',
  },
  {
    accessorKey: 'status',
    header: ({ column }) => (
      <SortableHeader column={column}>Status</SortableHeader>
    ),
    cell: ({ row }) => row.getValue('status') || UNKNOWN,
  },
  {
    accessorKey: 'binaryHash',
    header: 'Binary Hash',
    cell: ({ row }) => row.getValue('binaryHash') || UNKNOWN,
  },
  {
    accessorKey: 'configHash',
    header: 'Config Hash',
    cell: ({ row }) => row.getValue('configHash') || UNKNOWN,
  },
  {
    accessorKey: 'versionStatus',
    header: ({ column }) => (
      <SortableHeader column={column}>Version Status</SortableHeader>
    ),
    cell: ({ row }) => row.getValue('versionStatus') || UNKNOWN,
  },
  {
    id: 'actions',
    cell: ActionsMenu,
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
