'use client';

import { useState } from 'react';
import { useQuery } from 'react-query';
import { LevitationHeightResponse } from '@hyped/telemetry-types';

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
  const [show, setShow] = useState(false);

  const { data, error } = useQuery<LevitationHeightResponse>(
    'levitation-height',
    async () =>
      await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/levitation-height?start=0`,
      ).then((res) => res.json()),
    {
      refetchInterval: 1000,
    },
  );

  if (!data) return 'An error occurred';

  if (Object.keys(data)[0] === 'statusCode') {
    const error = true;
  }

  return (
    <Card decoration="top" decorationColor="red" className="h-[560px]">
      <Flex alignItems="start">
        <Text>Levitation Height</Text>
        <BadgeDelta
          deltaType={
            data[Object.keys(data)[0]][0]?.value > 0
              ? 'moderateIncrease'
              : 'moderateDecrease'
          }
        >
          Elevated
        </BadgeDelta>
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
      {data ? (
        Object.keys(data)
          .slice(0, Object.keys(data).length - 2)
          .map((height) => (
            <div key={height} className="space-y-2 mt-4">
              <Flex>
                <Text>{height}</Text>
                <Text>{`${
                  data[height][0]?.value ? data[height][0].value : 0
                } mm`}</Text>
              </Flex>

              <ProgressBar value={data[height][0]?.value}></ProgressBar>
            </div>
          ))
      ) : (
        <div>no data </div>
      )}

      {show ? (
        <div>
          {Object.keys(data)
            .slice(-2)
            .map((height) => (
              <div key={height} className="space-y-2 mt-4">
                <Flex>
                  <Text>{height}</Text>
                  <Text>{`${
                    data[height][0]?.value ? data[height][0].value : 0
                  } mm`}</Text>
                </Flex>

                <ProgressBar value={data[height][0]?.value}></ProgressBar>
              </div>
            ))}
        </div>
      ) : null}
      <Flex className="mt-6 pt-4 border-t">
        <Button
          size="xs"
          variant="light"
          //   icon={ArrowNarrowRightIcon}
          iconPosition="right"
          onClick={() => {
            !show ? setShow(true) : setShow(false);
          }}
        >
          View more
        </Button>
      </Flex>

      {show ? null : !(Object.keys(data)[0] === 'statusCode') ? (
        <div className="grid grid-cols-2 gap-2 p-5 ml-[-14px] levitation-badges">
          {Object.keys(data).map((height, i) => (
            <BadgeDelta
              key={i}
              deltaType={
                data[height][0]?.value > 0
                  ? 'moderateIncrease'
                  : 'moderateDecrease'
              }
            >
              Sensor {i}:{' '}
              {data[height][0]?.value > 0 ? 'Elevated' : 'disconnected'}
            </BadgeDelta>
          ))}
        </div>
      ) : (
        <div className="grid grid-cols-2 gap-2 p-5 ml-[-14px] levitation-badges">
          <BadgeDelta key={1} deltaType="moderateDecrease">
            Disconnected
          </BadgeDelta>
          <BadgeDelta key={2} deltaType="moderateDecrease">
            Disconnected
          </BadgeDelta>
          <BadgeDelta key={3} deltaType="moderateDecrease">
            Disconnected
          </BadgeDelta>
          <BadgeDelta key={4} deltaType="moderateDecrease">
            Disconnected
          </BadgeDelta>
          <BadgeDelta key={5} deltaType="moderateDecrease">
            Disconnected
          </BadgeDelta>
          <BadgeDelta key={6} deltaType="moderateDecrease">
            Disconnected
          </BadgeDelta>
        </div>
      )}
    </Card>
  );
}
