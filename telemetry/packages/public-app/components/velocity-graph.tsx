'use client';

import { getVelocity } from '@/helpers';
import { HistoricalValueResponse } from '@hyped/telemetry-types';
import { Card, Title, LineChart } from '@tremor/react';
import { format } from 'date-fns';
import { useQuery } from 'react-query';

/**
 * The velocity chart. Fetches velocity data from the server.
 * @returns The velocity chart.
 */
export const VelocityGraph = () => {
  const { data } = useQuery('velocity', async () => await getVelocity(), {
    refetchInterval: 1000,
  });

  const velocityData = formatData(data);

  return (
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
          velocity: d.value,
        };
      })
    : [];
