import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from '@/components/ui/table';
import {
  SortingState,
  flexRender,
  getCoreRowModel,
  getPaginationRowModel,
  getSortedRowModel,
  useReactTable,
} from '@tanstack/react-table';
import { Cpu, HardDriveUpload, RefreshCw } from 'lucide-react';
import { http } from 'openmct/core/http';
import { useState } from 'react';
import { useQuery } from 'react-query';
import { Button } from '../ui/button';
import { columns } from './columns';
import { cn } from '@/lib/utils';
import { PiInfo } from '@hyped/telemetry-types';

// temp until debug view is merged
const POD_ID = 'pod_1';

export const PiManagement = () => {
  const [sorting, setSorting] = useState<SortingState>([]);
  const [rowSelection, setRowSelection] = useState({});

  const { data, isLoading, isRefetching, refetch } = useQuery(
    'pis',
    async () => {
      const res = await http.get(`pods/${POD_ID}/pis`).then((res) => res.json());
      return res as PiInfo[]
    },
  );

  const table = useReactTable({
    data: data || [],
    columns,
    getCoreRowModel: getCoreRowModel(),
    getPaginationRowModel: getPaginationRowModel(),
    onSortingChange: setSorting,
    getSortedRowModel: getSortedRowModel(),
    onRowSelectionChange: setRowSelection,
    state: {
      sorting,
      rowSelection,
    },
  });

  const numSelectedRows = table.getFilteredSelectedRowModel().rows.length;

  return (
    <div className="max-w-6xl mx-auto flex flex-col h-full gap-4 p-16">
      <h1 className="text-4xl font-bold mb-8 flex gap-4 items-center">
        <Cpu size={32} />
        Pi Management Console
      </h1>
      <div className="flex justify-between items-center">
        <Button
          variant="outline"
          onClick={() => void refetch()}
          className="flex gap-2 group text-base"
        >
          <RefreshCw size={16} className={cn(isRefetching && 'animate-spin')} />
          Refresh
        </Button>
        <div className="flex gap-4">
          <Button variant="outline" className="flex gap-2">
            <HardDriveUpload size={16} />
            Update all
          </Button>
          <Button className="flex gap-2" disabled={numSelectedRows < 1}>
            <HardDriveUpload size={16} />
            Update selected ({numSelectedRows})
          </Button>
        </div>
      </div>
      <div>
        <div className="rounded-md border">
          <Table>
            <TableHeader>
              {table.getHeaderGroups().map((headerGroup) => (
                <TableRow key={headerGroup.id}>
                  {headerGroup.headers.map((header) => {
                    return (
                      <TableHead key={header.id}>
                        {header.isPlaceholder
                          ? null
                          : flexRender(
                              header.column.columnDef.header,
                              header.getContext(),
                            )}
                      </TableHead>
                    );
                  })}
                </TableRow>
              ))}
            </TableHeader>
            <TableBody>
              {table.getRowModel().rows?.length ? (
                table.getRowModel().rows.map((row) => (
                  <TableRow
                    key={row.id}
                    data-state={row.getIsSelected() && 'selected'}
                  >
                    {row.getVisibleCells().map((cell) => (
                      <TableCell key={cell.id}>
                        {flexRender(
                          cell.column.columnDef.cell,
                          cell.getContext(),
                        )}
                      </TableCell>
                    ))}
                  </TableRow>
                ))
              ) : (
                <TableRow>
                  <TableCell
                    colSpan={columns.length}
                    className="h-24 text-center"
                  >
                    {isLoading ? 'Loading...' : 'No data'}
                  </TableCell>
                </TableRow>
              )}
            </TableBody>
          </Table>
        </div>
        <div className="flex items-start justify-between space-x-2 py-4">
          <div className="flex-1 text-sm text-muted-foreground">
            {table.getFilteredSelectedRowModel().rows.length} of{' '}
            {table.getFilteredRowModel().rows.length} row(s) selected.
          </div>
          <Button
            variant="outline"
            size="sm"
            onClick={() => table.previousPage()}
            disabled={!table.getCanPreviousPage()}
          >
            Previous
          </Button>
          <Button
            variant="outline"
            size="sm"
            onClick={() => table.nextPage()}
            disabled={!table.getCanNextPage()}
          >
            Next
          </Button>
        </div>
      </div>
    </div>
  );
};
