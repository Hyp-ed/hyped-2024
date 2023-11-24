import { Card, Metric, Text } from '@tremor/react';
import { useSignal, useComputed } from '@preact/signals-react';
import { useEffect, useState } from 'react';
import { Badge, BadgeDelta, Callout } from '@tremor/react';

export function DigitalTimer(): JSX.Element {
  let count: number = 0;
  const [time, setTime] = useState(count);
  useEffect(() => {
    const timerId = setInterval(() => {
      count += 1;

      console.log(count);

      setTime(count);
    }, 1000);

    // Cleanup function to clear the interval when the component is unmounted
    return () => clearInterval(timerId);
  }, [count]);

  return (
    <Card
      // className="max-w-xs mx-auto"
      decoration="top"
      decorationColor="red"
    >
      <div className="timer">
        <div>
          <Text>Time since launch</Text>
          <Metric>
            {Math.floor(time / 60)}m {time % 60}s
          </Metric>
        </div>

        <div>
          <BadgeDelta size="lg">LIVE</BadgeDelta>
        </div>
      </div>
      <div className="flex flex-row flex-wrap gap-3">
        {' '}
        <Callout
          className="h-15 mt-3"
          title="Status: Deployed but not running"
          // icon={ExclamationIcon}
          color="rose"
        >
          Unknown error present. Consult technical team.
        </Callout>
        <Callout
          className="h-15 mt-3"
          title="Status: System online"
          // icon={ExclamationIcon}
          color="green"
        ></Callout>
        <Callout
          className="h-15 mt-3"
          title="Status: Pod Stationary"
          // icon={ExclamationIcon}
          color="green"
        ></Callout>
      </div>
    </Card>
  );
}
