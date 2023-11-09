'use client';

import { Card, Title, LineChart } from '@tremor/react';
import { format } from 'date-fns';
import { useQuery } from 'react-query';

// const chartdata3 = [
//   {
//     date: 'Jan 23',
//     running: 167,
//   },
//   // ...
//   {
//     date: 'Sep 23',
//     running: 132,
//   },
// ];

export const VelocityGraph = () => {
  const { data, isLoading, isError } = useQuery(
    'velocity',
    async () =>
      (await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/velocity?start=0`,
      ).then(res => res.json())) as {
        id: 'velocity';
        timestamp: string;
        value: number;
      }[],
    {
      refetchInterval: 1000,
    },
  );

  const velocityData = data
    ? data.map(d => {
        const time = new Date(d.timestamp);
        return {
          time: format(time, 'HH:mm:ss'),
          velocity: d.value,
        };
      })
    : [];

  // if (isError) return <div>Error loading velocity</div>;

  // if (isLoading) return <div>Loading...</div>;

  return (
    <>
      <Card className="v-graph dark:bg-black">
        <Title className="">Velocity</Title>
        <LineChart
          className="h-72 mt-6 dark:text-white"
          data={velocityData}
          // data={chartdata3}
          index="time"
          categories={['velocity']}
          colors={['red']}
          yAxisWidth={30}
        />
      </Card>
    </>
  );
};
