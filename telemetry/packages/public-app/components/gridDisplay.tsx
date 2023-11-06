import { Card, Title, Text, Grid } from '@tremor/react';
import { VelocityGraph } from './velocity-graph';
import KpiCard from './test';
import { TrackerExample } from './tracker';
import { useState } from 'react';
import { MeasurementChart } from './measurementsGraph';
import { AccelerationChart } from './accelerationChart';
import LevitationBar from './levitationBar';
import Image from 'next/image';

const CARDS = {
  //VELOCITY: <VelocityGraph />,
  //KPI_CARD: <KpiCard />,
  // TRACKER: <TrackerExample />,
  //MEASUREMENTS: <MeasurementChart />,
  VELOCITY: <VelocityGraph />,
  ACCELERATION: <AccelerationChart />,
  LEVITATION: <LevitationBar />,
};

type Card = keyof typeof CARDS;

export default function GridDsiplay() {
  const ColorInverter = () => {};

  const [selected, setSelected] = useState<Card>('VELOCITY');

  return (
    <main className="invert-mode">
      <div className="heading p-2">
        <div>
          <Image
            alt="Hyped logo"
            src="/hyped.svg"
            width="50"
            height="50"
            className="mt-2"
          />
        </div>
        <div>
          {' '}
          <Title className="invert-mode">Dashboard</Title>
          <Text className="invert-mode">
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr.
          </Text>
          <button className="invert-mode" onClick={ColorInverter}>
            press
          </button>
        </div>
      </div>

      {/* Main section */}
      <Card
        className="mt-6 
        w-[360px]

      sm:w-[360px]
       md:w-[720px]
        lg:w-[1000px] 
        grid-display"
      >
        <div className="h-0" />
        {CARDS[selected]}
      </Card>

      {/* KPI section */}
      <Grid
        numItemsMd={2}
        className="mt-6 gap-6 w-[360px] md:w-[720px] lg:w-[1000px]"
      >
        {(Object.keys(CARDS) as Card[])
          .filter(c => c !== selected)
          .map(c => (
            <button key={c} onClick={() => setSelected(c)}>
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
