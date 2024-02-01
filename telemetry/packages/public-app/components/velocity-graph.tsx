'use client';

import { Card, Title, LineChart } from '@tremor/react';
import { format } from 'date-fns';
import { useQuery } from 'react-query';

export const VelocityGraph = () => {
  const { data, isLoading, isError } = useQuery(
    'velocity',
    async () =>
      (await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/velocity?start=0`,
      ).then((res) => res.json())) as {
        id: 'velocity';
        timestamp: string;
        value: number;
      }[],
    {
      refetchInterval: 1000,
    },
  );

  const velocityData = Array.isArray(data)
    ? data.map((d) => {
        const time = new Date(d.timestamp);
        return {
          time: format(time, 'HH:mm:ss'),
          velocity: d.value,
        };
      })
    : [];

  return (
    <>
      <Card className="v-graph" decoration="top" decorationColor="red">
        <Title className="">Velocity</Title>
        <LineChart
          className="h-[420px] mt-6 dark:text-white"
          data={velocityData}
          index="time"
          categories={['velocity']}
          colors={['red']}
          yAxisWidth={30}
        />
      </Card>
    </>
  );
};
