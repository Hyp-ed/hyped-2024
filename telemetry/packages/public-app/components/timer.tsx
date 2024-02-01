import { Card, Metric, Text } from '@tremor/react';

import { useEffect, useState } from 'react';
import { BadgeDelta } from '@tremor/react';
import { useQuery } from 'react-query';

export function DigitalTimer(): JSX.Element {
  const { data } = useQuery(
    'launch-time',
    async () =>
      (await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/launch-time`,
      ).then((res) => res.json())) as any,
    {
      refetchInterval: 1000,
    },
  );

  const launchTime = data?.launchTime || -1;

  const [time, setTime] = useState(-1);

  useEffect(() => {
    const d = new Date();
    const now = d.getTime();
    console.log(now);
    setTime(launchTime == -1 ? -1 : (now - launchTime) / 1000);
  }, [launchTime]);

  useEffect(() => {
    const interval = setInterval(() => {
      setTime((time) => (time == -1 ? time : time + 1));
    }, 1000);

    return () => clearInterval(interval);
  }, []);

  return (
    <Card
      // className="max-w-xs mx-auto"
      decoration="top"
      decorationColor="red"
      className="w-[50%] timer-card"
    >
      <div className="timer">
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
      </div>
    </Card>
  );
}
