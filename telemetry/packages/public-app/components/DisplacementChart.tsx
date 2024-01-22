'use client';

import { Card, LineChart, Title } from '@tremor/react';
import { useQuery } from 'react-query';
import format from 'date-fns/format';

const valueFormatter = (number: any) =>
  `$ ${new Intl.NumberFormat('uk').format(number).toString()}`;

export const DisplacementChart = () => {
  const { data, isLoading, isError } = useQuery(
    'displacement',
    async () =>
      (await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/displacement?start=0`,
      ).then((res) => res.json())) as {
        id: 'displacement';
        timestamp: string;
        value: number;
      }[],
    {
      refetchInterval: 1000,
    },
  );

  const displacementData = Array.isArray(data)
    ? data.map((d) => {
        const time = new Date(d.timestamp);
        return {
          time: format(time, 'HH:mm:ss'),
          displacement: d.value,
        };
      })
    : [];


  return (
    <Card className="h-[560px]" decoration="top" decorationColor="red">
      <Title>Displacement</Title>

      <LineChart
        className="h-[420px] mt-6"
        data={displacementData}
        index="year"
        categories={['displacement']}
        colors={['red']}
        yAxisWidth={40}
      />
    </Card>
  );
};
