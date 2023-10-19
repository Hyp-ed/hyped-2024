import { Card, Title, Tracker, Flex, Text, Color } from '@tremor/react';

interface Tracker {
  color: Color;
  tooltip: string;
}

const data: Tracker[] = [
  { color: 'rose', tooltip: 'Operational' },
  { color: 'rose', tooltip: 'Operational' },
  { color: 'rose', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'rose', tooltip: 'Downtime' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'gray', tooltip: 'Maintenance' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'emerald', tooltip: 'Operational' },
  { color: 'yellow', tooltip: 'Degraded' },
  { color: 'emerald', tooltip: 'Operational' },
];

export const TrackerExample = () => (
  <Card className="max-w-sm mx-auto">
    <Title>Status</Title>
    <Text>Lena&apos;s Webshop &bull; May 2022</Text>
    <Flex justifyContent="end" className="mt-4">
      <Text>Uptime 92%</Text>
    </Flex>
    <Tracker data={data} className="mt-2" />
  </Card>
);
