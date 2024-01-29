import { PiWithVersion } from '@hyped/telemetry-types';
import { ColumnDef } from '@tanstack/react-table';
import { ActionsMenu } from './actions-menu';

const UNKNOWN = 'unknown';

export const columns: ColumnDef<PiWithVersion>[] = [
  {
    accessorKey: 'id',
    header: 'ID',
  },
  {
    accessorKey: 'name',
    header: 'Name',
  },
  {
    accessorKey: 'ip',
    header: 'IP Address',
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
    accessorKey: 'status',
    header: 'Status',
    cell: ({ row }) => row.getValue('status') || UNKNOWN,
  },
  {
    id: 'actions',
    cell: ActionsMenu,
  },
];
