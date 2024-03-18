'use client';

import { Card, LineChart, Title } from '@tremor/react';
import { useQuery } from 'react-query';
import format from 'date-fns/format';
import { getDisplacement } from '@/helpers';
import { HistoricalValueResponse } from '@hyped/telemetry-types';

/**
 * The displacement chart. Fetches displacement data from the server.
 * @returns The displacement chart.
 */
export const DisplacementChart = () => {
  const { data } = useQuery(
    'displacement',
    async () => await getDisplacement(),
    {
      refetchInterval: 1000,
    },
  );

  const displacementData = formatData(data);

  return (
    <Card decoration="top" decorationColor="red">
      <Title>Displacement</Title>
      <LineChart
        className="h-[420px] mt-6"
        data={displacementData}
        index="time"
        categories={['displacement']}
        colors={['red']}
        yAxisWidth={40}
      />
    </Card>
  );
};

/**
 * Formats the data to be displayed on the chart.
 * @param data The data to format.
 * @returns The formatted data.
 */
const formatData = (data: HistoricalValueResponse | undefined) =>
  data
    ? data.map((d) => {
        const time = new Date(d.timestamp);
        return {
          time: format(time, 'HH:mm:ss'),
          displacement: d.value,
        };
      })
    : [];
