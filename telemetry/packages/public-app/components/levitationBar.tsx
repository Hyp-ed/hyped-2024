'use client';

import { Badge, Card, Flex, MarkerBar, Text, Title } from '@tremor/react';
import { useQuery } from 'react-query';
import format from 'date-fns/format';

export default () => {
  const { data, isLoading, isError } = useQuery(
    'levitation-height',
    async () =>
      (await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/levitation-height?start=0`,
      ).then(res => res.json())) as {
        id: 'levitation-height';
        timestamp: string;
        value: number;
      }[],
    {
      refetchInterval: 1000,
    },
  );

  const levitationHeight = Array.isArray(data)
    ? data.map(d => {
        return {
          levitation: d.value,
        };
      })
    : [];
  const levitation1 = levitationHeight[levitationHeight.length - 1]?.levitation;
  console.log(levitation1);

  return (
    <Card className="levitate-card dark:bg-black">
      <Title className="mb-[-10px] z-10"> Levitation Height</Title>
      <div className="levitate">
        <div className="levitation">
          <Card
            className=" 
          levitate-1
         lg:w-[230px] pt-[160px]  h-[330px]
         dark:bg-black
        "
          >
            <div className="levitate-2">
              <Text className="mb-3">Levitation Height</Text>
              <Badge>live</Badge>
              <Text>{levitation1}mm</Text>
            </div>
            <MarkerBar
              value={levitation1}
              color="fuchsia"
              className="mt-4 marker-bar"
            />
          </Card>
        </div>
      </div>
    </Card>
  );
};
