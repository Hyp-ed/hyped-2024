import { Card, Title, Text, Grid } from '@tremor/react';
import { VelocityGraph } from './velocity-graph';
import KpiCard from './test';
import { TrackerExample } from './tracker';
import { useState } from 'react';

const CARDS = {
  VELOCITY: <VelocityGraph />,
  KPI_CARD: <KpiCard />,
  TRACKER: <TrackerExample />,
};

type Card = keyof typeof CARDS;

export default function GridDsiplay() {
  const ColorInverter = () => {
    document.documentElement.style.setProperty(
      '--bg-color',
      document.documentElement.style.getPropertyValue('--bg-color') === 'black'
        ? 'white'
        : 'black',
    );
    document.documentElement.style.setProperty(
      '--invert-color',
      document.documentElement.style.getPropertyValue('--bg-color') === 'black'
        ? 'white'
        : 'black',
    );
  };

  const [selected, setSelected] = useState<Card>('VELOCITY');

  return (
    <main className="invert-mode">
      <Title className="invert-mode">Dashboard</Title>
      <Text className="invert-mode">
        Lorem ipsum dolor sit amet, consetetur sadipscing elitr.
      </Text>
      <button className="invert-mode" onClick={ColorInverter}>
        press
      </button>

      {/* Main section */}
      <Card className="mt-6 sm:w-[370px] md:w-[640px] lg:w-[900px]">
        <div className="h-0" />
        {CARDS[selected]}
      </Card>

      {/* KPI section */}
      <Grid numItemsMd={2} className="mt-6 gap-6">
        {(Object.keys(CARDS) as Card[])
          .filter((c) => c !== selected)
          .map((c) => (
            <button onClick={() => setSelected(c)}>
              {/* <Card> */}
                {/* Placeholder to set height */}
                <div className="h-0" />
                {CARDS[c]}
              {/* </Card> */}
            </button>
          ))}
      </Grid>
    </main>
  );
}
