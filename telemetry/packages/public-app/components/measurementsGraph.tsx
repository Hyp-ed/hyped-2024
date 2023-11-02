'use client';
import format from 'date-fns/format';
import { AreaChart, Card, Title } from '@tremor/react';
import { useQuery } from 'react-query';
import { Badge, BadgeDelta } from '@tremor/react';

// const chartdata3 = [
//   {
//     date: "Jan 23",
//     Running: 167,
//     Cycling: 145,
//   },
//   {
//     date: "Feb 23",
//     Running: 125,
//     Cycling: 110,
//   },
//   {
//     date: "Mar 23",
//     Running: 156,
//     Cycling: 149,
//   },
//   {
//     date: "Apr 23",
//     Running: 165,
//     Cycling: 112,
//   },
//   {
//     date: "May 23",
//     Running: 153,
//     Cycling: 138,
//   },
//   {
//     date: "Jun 23",
//     Running: 124,
//     Cycling: 145,
//   },
//   {
//     date: "Jul 23",
//     Running: 164,
//     Cycling: 134,
//   },
//   {
//     date: "Aug 23",
//     Running: 123,
//     Cycling: 110,
//   },
//   {
//     date: "Sep 23",
//     Running: 132,
//     Cycling: 113,
//   },
// ];
interface Category {
  color: string;
  dataKey: string;
  value: number;
}
type Payload = Category[];

type Active = boolean;

const customTooltip: any = ({
  payload,
  active,
}: {
  payload: Payload;
  active: Active;
}) => {
  if (!active || !payload) return null;
  return (
    <div className="w-56 rounded-tremor-default text-tremor-default bg-tremor-background p-2 shadow-tremor-dropdown border border-tremor-border">
      {payload.map((category, idx) => (
        <div key={idx} className="flex flex-1 space-x-2.5">
          <div
            className={`w-1 flex flex-col bg-${category.color}-500 rounded`}
          />
          <div className="space-y-1">
            <p className="text-tremor-content">{category.dataKey}</p>
            <p className="font-medium text-tremor-content-emphasis">
              {category.value} bpm
            </p>
          </div>
        </div>
      ))}
    </div>
  );
};

export const MeasurementChart = () => {
  const { data: velocityData } = useQuery(
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

  const { data: displacementData } = useQuery(
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

  const measurementData = [
    ...(velocityData
      ? velocityData.map((d) => {
          const time = new Date(d.timestamp);
          return {
            time: format(time, 'HH:mm:ss'),
            velocity: d.value,
            displacement: null,
          };
        })
      : []),
    ...(displacementData
      ? displacementData.map((d) => {
          const time = new Date(d.timestamp);
          return {
            time: format(time, 'HH:mm:ss'),
            displacement: d.value,
            velocity: null,
          };
        })
      : []),
  ].sort((a, b) => {
    if (new Date(a.time) < new Date(b.time)) return 1;
    return -1;
  });

  console.log(measurementData);

  //   if (isError) return <div>Error loading velocity</div>;

  //   if (isLoading) return <div>Loading...</div>;

  return (
    <>
      <Card>
        <Badge>live</Badge>
        <Title>Average BPM</Title>
        <AreaChart
          className="h-72 mt-4"
          data={measurementData}
          index="time"
          categories={['velocity', 'displacement']}
          colors={['blue', 'red']}
          yAxisWidth={30}
          customTooltip={customTooltip}
        />
      </Card>
    </>
  );
};
