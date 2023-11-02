'use client';

import {
  BadgeDelta,
  Card,
  Flex,
  Metric,
  ProgressBar,
  Text,
} from '@tremor/react';
import { TrackerExample } from './tracker';

// Single KPI card in the demo dashboard with sample inputs
export default function KpiCard() {
  return (
    <Card className=" combined">
      <Card className="max-w-lg mx-auto">
        <Flex alignItems="start">
          <div>
            <Text>Sales</Text>
            <Metric>$ 12,699</Metric>
          </div>
          <BadgeDelta deltaType="moderateIncrease">13.2%</BadgeDelta>
        </Flex>

        <Flex className="mt-4">
          <Text className="truncate">68% ($ 149,940)</Text>
          <Text>$ 220,500</Text>
        </Flex>

        <ProgressBar value={15.9} className="mt-2" />
      </Card>
      <TrackerExample/>
    </Card>
  );
}
