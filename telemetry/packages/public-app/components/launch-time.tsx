'use client';

import { Card, Metric, Text } from '@tremor/react';
import { useEffect, useState } from 'react';
import { BadgeDelta } from '@tremor/react';
import { useQuery } from 'react-query';
import { getLaunchTime } from '@/helpers';

/**
 * The launch time card. Fetches the launch time from the server.
 * @returns The launch time card.
 */
export function LaunchTime() {
  const { data } = useQuery('launch-time', async () => await getLaunchTime(), {
    refetchInterval: 1000,
  });

  const launchTime = data?.launchTime ?? -1;

  const [time, setTime] = useState(-1);

  useEffect(() => {
    const d = new Date();
    const now = d.getTime();
    setTime(launchTime == -1 ? -1 : (now - launchTime) / 1000);
  }, [launchTime]);

  return (
    <Card decoration="top" decorationColor="red">
      <div>
        <Text>Time since launch</Text>
        <Metric>
          {time > -1
            ? `${Math.floor(time / 60)}m ${Math.floor(time % 60)}s`
            : 'Not launched yet'}
        </Metric>
      </div>
      <div>
        <BadgeDelta
          size="lg"
          deltaType={launchTime > 0 ? 'moderateIncrease' : 'moderateDecrease'}
        >
          {launchTime > 0 ? 'LIVE' : 'Not Launched'}
        </BadgeDelta>
      </div>
    </Card>
  );
}
