'use client';

import { Card, LineChart, Title } from '@tremor/react';
import { useQuery } from 'react-query';
import format from 'date-fns/format';
import { getDisplacement } from '@/helpers';

export const DisplacementChart = () => {
  const { data } = useQuery(
    'displacement',
    async () => await getDisplacement(),
    {
      refetchInterval: 1000,
    },
  );

  const displacementData = data
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
