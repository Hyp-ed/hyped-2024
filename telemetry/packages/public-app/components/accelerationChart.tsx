import { Card, LineChart, Title } from '@tremor/react';
import { useQuery } from 'react-query';
import format from 'date-fns/format';

const valueFormatter = (number: any) =>
  `$ ${new Intl.NumberFormat('us').format(number).toString()}`;

export const AccelerationChart = () => {
  const { data, isLoading, isError } = useQuery(
    'displacement    ',
    async () =>
      (await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/displacement?start=0`,
      ).then(res => res.json())) as {
        id: 'displacement';
        timestamp: string;
        value: number;
      }[],
    {
      refetchInterval: 1000,
    },
  );

  const displacementData = data
    ? data.map(d => {
        const time = new Date(d.timestamp);
        return {
          time: format(time, 'HH:mm:ss'),
          displacement: d.value,
        };
      })
    : [];

  //if (isLoading) return <div>Currently loading acceleration...</div>;
  return (
    <Card>
      <Title>Export/Import Growth Rates (1970 to 2021)</Title>

      <LineChart
        className="mt-6"
        data={displacementData}
        index="year"
        categories={['acceleration']}
        colors={['red']}
        valueFormatter={valueFormatter}
        yAxisWidth={40}
      />
    </Card>
  );
};
