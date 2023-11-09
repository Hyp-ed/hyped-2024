import { Card, Title, Text, Grid } from '@tremor/react';
import { VelocityGraph } from './velocity-graph';

import { TrackerExample } from './tracker';
import { useState } from 'react';
import { MeasurementChart } from './measurementsGraph';
import { DisplacementChart } from './DisplacementChart';
import LevitationBar from './levitationBar';
import Image from 'next/image';
import { useEffect } from 'react';

const CARDS = {
  VELOCITY: <VelocityGraph />,
  ACCELERATION: <DisplacementChart />,
  LEVITATION: <LevitationBar />,
};

type Card = keyof typeof CARDS;

const ColorInverter = () => {
  if (localStorage.getItem('color-theme')) {
    if (localStorage.getItem('color-theme') === 'light') {
      localStorage.setItem('color-theme', 'dark');
      document.documentElement.classList.remove('light');
      document.documentElement.classList.add('dark');
    } else if (localStorage.getItem('color-theme') === 'dark') {
      localStorage.setItem('color-theme', 'light');
      document.documentElement.classList.add('light');
      document.documentElement.classList.remove('dark');
    }
  } else {
    localStorage.setItem('color-theme', 'dark');
    document.documentElement.classList.add('dark');
  }
};

const refreshStorage = () => {
  localStorage.clear;
};
window.onload = refreshStorage;
export default function GridDsiplay() {
  const [selected, setSelected] = useState<Card>('VELOCITY');

  return (
    <main className="w-[360px] md:w-[720px] lg:w-[1000px] mx-auto py-12">
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
          <Title>Dashboard</Title>
          <Text>Lorem ipsum dolor sit amet, consetetur sadipscing elitr.</Text>
          <button onClick={ColorInverter} className="dark:text-white">
            {' '}
            MODE
          </button>
        </div>
      </div>

      <div className="mt-5 top-card"> {CARDS[selected]}</div>
      {/* KPI section */}
      <div className="top-card">
        <Grid numItemsMd={2} className="mt-6 gap-6 w-full">
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
      </div>
    </main>
  );
}
