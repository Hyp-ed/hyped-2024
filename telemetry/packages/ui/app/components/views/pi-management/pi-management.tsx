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
import { Button } from '../../ui/button';
import { columns } from './columns';
import { cn } from '@/lib/utils';
import { PiInfo } from '@hyped/telemetry-types';
import { useCurrentPod } from '@/context/pods';
import { SelectBranch } from './select-branch';

const getPis = async (
  podId: string,
  compareBranch: string,
): Promise<PiInfo[]> => {
  const res = await http
    .get(`pods/${podId}/pis?compareBranch=${compareBranch}`)
    .then((res) => res.json());
  return res as PiInfo[];
};

export const DEFAULT_BRANCH = 'master';

// TODO: Give an option of the branches on the GitHub to select from (for Pi version comparison)

export const PiManagement = () => {
  // const { currentPod } = useCurrentPod();
  const currentPod = 'pod_2024';

  const [compareBranch, setCompareBranch] = useState<string>(DEFAULT_BRANCH);

  const [sorting, setSorting] = useState<SortingState>([]);
  const [rowSelection, setRowSelection] = useState({});

  const { data, isLoading, isRefetching, refetch } = useQuery(
    ['pis', currentPod],
    () => getPis(currentPod, compareBranch),
  );

  const table = useReactTable({
    data: data || [],
    columns: columns(compareBranch),
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
    <div className="flex flex-col h-full gap-4 p-16">
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
          <div className="flex gap-4 items-center">
            Compare to...
            <SelectBranch setCompareBranch={setCompareBranch} />
          </div>
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
