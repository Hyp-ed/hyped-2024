import { http } from 'openmct/core/http';
import { useQuery } from 'react-query';
import { DataTable } from './data-table';
import { columns } from './columns';
import { PiWithVersion } from '@hyped/telemetry-types';
import { Button } from '../ui/button';
import { Cpu, HardDriveUpload, RefreshCw } from 'lucide-react';
import { cn } from '@/lib/utils';

// temp until debug view is merged
const POD_ID = 'pod_1';

export const PiManagement = () => {
  const { data, isLoading, isError, isRefetching, refetch } = useQuery(
    'pis',
    () =>
      http.get(`pods/${POD_ID}/pis`).then((res) => res.json()) as Promise<
        PiWithVersion[]
      >,
  );

  return (
    <div className="max-w-6xl mx-auto flex flex-col h-full gap-4 p-16">
      <h1 className="text-4xl font-bold mb-8 flex gap-4 items-center">
        <Cpu size={32} />
        Pi Management Console
      </h1>
      <div className="flex justify-between items-center">
        <Button
          variant="outline"
          onClick={() => refetch()}
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
          <Button className="flex gap-2">
            <HardDriveUpload size={16} />
            Update selected
          </Button>
        </div>
      </div>
      <DataTable columns={columns} data={data || []} isLoading={isLoading} />
    </div>
  );
};
