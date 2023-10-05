import { PreviousLatenciesType } from '@/context/pods';
import { Title, LineChart } from '@tremor/react';

const dataFormatter = (number: number) => `${number.toString()}ms`;

export const LatencyChart = ({
  data,
}: {
  data: PreviousLatenciesType | undefined;
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
          maxValue={100}
          minValue={0}
        />
      ) : (
        <p>No latencies to show</p>
      )}
    </div>
  );
};
