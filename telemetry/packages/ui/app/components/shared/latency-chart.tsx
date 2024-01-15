import { PreviousLatenciesType } from '@/context/pods';
import { LineChart } from '@tremor/react';

const dataFormatter = (number: number) => `${number.toString()}ms`;

export const LatencyChart = ({
  data,
  minValue = 0,
  maxValue = 100,
}: {
  data: PreviousLatenciesType | undefined;
  minValue?: number;
  maxValue?: number;
}) => {
  return (
    <div className="w-[200px]">
      {data && data.length > 0 ? (
        <LineChart
          className="h-[70px]"
          data={data}
          index="time"
          categories={['latency']}
          colors={['blue']}
          valueFormatter={dataFormatter}
          yAxisWidth={40}
          showYAxis={true}
          showXAxis={false}
          showLegend={false}
          showGridLines={true}
          maxValue={maxValue}
          minValue={minValue}
        />
      ) : (
        <p className="text-sm">No latencies to show</p>
      )}
    </div>
  );
};
