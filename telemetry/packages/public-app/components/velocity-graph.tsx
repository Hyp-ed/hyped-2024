import { Card, Title, LineChart } from '@tremor/react';
import inFocus from './gridDisplay';

const chartdata3 = [
  {
    date: 'Jan 23',
    velocity: 167,
  },
  // ...
  {
    date: 'Sep 23',
    velocity: 132,
  },
  {
    date: 'Sep 24',
    velocity: 145,
  },
  {
    date: 'Sep 25',
    velocity: 155,
  },
];
// export var inversion = (document.documentElement.style.getPropertyValue('--bg-color')=='black')?('inverted'):('')
//export var inversion1 = document.documentElement.style.getPropertyValue('--bg-color')=='black'?('inverted'):('')
export const VelocityGraph = () => {
  return (
    <>
      <Card className="v-graph" onClick={inFocus}>
        <Title>Velocity</Title>
        <LineChart
          className="h-72 mt-4"
          data={chartdata3}
          index="date"
          categories={['velocity']}
          colors={['red']}
          yAxisWidth={30}
        />
      </Card>
    </>
  );
};
