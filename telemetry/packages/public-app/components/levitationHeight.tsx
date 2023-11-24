'use client';

import { useState } from 'react';
import { useQuery } from 'react-query';

import {
  Card,
  TabList,
  Tab,
  ProgressBar,
  Text,
  Flex,
  Button,
  Metric,
  BadgeDelta,
  TabGroup,
} from '@tremor/react';

interface products {
  title: string;
  value: number | null;
  metric: string;
  location: string;
}

const products = [
  {
    title: 'Levitation: Pod 1',
    value: 0,
    metric: 'mm',
    location: 'A',
  },
  {
    title: 'Levitation: Pod 2',
    value: 0,
    metric: 'mm',
    location: 'A',
  },
  {
    title: 'Levitation: Pod 3',
    value: 0,
    metric: 'mm',
    location: 'A',
  },
  {
    title: 'Levitation: Pod 4',
    value: 0,
    metric: 'mm',
    location: 'B',
  },
];

export default function LevitationHeight() {
  const [selectedIndex, setSelectedIndex] = useState(0);
  const selectedLocation = selectedIndex === 0 ? 'A' : 'B';

  const { data } = useQuery(
    'levitation_height_1',
    async () =>
      (await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/levitation-height?start=0`,
      ).then((res) => res.json())) as {
        id: 'levitation_height_1';
        timestamp: string;
        value: number;
      }[],
    {
      refetchInterval: 1000,
    },
  );
  console.log(data);

  // for (let i = 0; i < products.length; i++) {
  //   products[i].value = data ? data[i].value : 0;
  // }

  return (
    <Card decoration="top" decorationColor="red" className="h-[410px]">
      <Flex alignItems="start">
        <Text>Levitation Height</Text>
        <BadgeDelta deltaType="moderateIncrease">Elevated</BadgeDelta>
      </Flex>
      <Flex
        justifyContent="start"
        alignItems="baseline"
        className="space-x-3 truncate"
      >
        <Metric>Pod Height</Metric>
        <Text></Text>
      </Flex>
      <TabGroup
        index={selectedIndex}
        onIndexChange={setSelectedIndex}
        className="mt-6"
      >
        <TabList>
          <Tab>Pod 1</Tab>
        </TabList>
      </TabGroup>
      {products
        .filter((item) => item.location === selectedLocation)
        .map((item) => (
          <div key={item.title} className="space-y-2 mt-4">
            <Flex>
              <Text>{item.title}</Text>
              <Text>{`${item.value}% (${item.metric})`}</Text>
            </Flex>
            <ProgressBar value={item.value} />
          </div>
        ))}
      <Flex className="mt-6 pt-4 border-t">
        <Button
          size="xs"
          variant="light"
          //   icon={ArrowNarrowRightIcon}
          iconPosition="right"
        >
          View more
        </Button>
      </Flex>
    </Card>
  );
}
