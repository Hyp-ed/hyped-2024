import { Card, Title, Text, Grid, Badge } from '@tremor/react';
import { VelocityGraph } from './velocity-graph';

import { useState } from 'react';

import { DisplacementChart } from './DisplacementChart';

import Image from 'next/image';

import { DigitalTimer } from './timer';
import Switch from 'react-switch';
import LevitationHeight from './levitationHeight';
import { SocialIcons } from './socialIcons';
import { Gauge } from './guage';

const CARDS = {
  VELOCITY: <VelocityGraph />,
  ACCELERATION: <DisplacementChart />,
  LEVITATION: <LevitationHeight />,
};

type Card = keyof typeof CARDS;

export const ColorInverter = () => {
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

export default function GridDsiplay() {
  const [selected, setSelected] = useState<Card>('VELOCITY');
  const [clicked, setClicked] = useState(false);
 
  const switched = () => {
    ColorInverter();
    !clicked && localStorage.getItem('color-theme') === 'dark'
      ? setClicked(true)
      : setClicked(false);
  };

  return (
    <main className="w-[360px] md:w-[720px] lg:w-[1000px] mx-auto py-12">
      <div className="nav p-2 mb-3">
        <div className="heading  p-2">
          {' '}
          <div>
            <Image
              alt="Hyped logo"
              src={
                clicked === false ? '/hypedLogoLight.png' : '/hypedLogoDark.png'
              }
              width="200"
              height="50"
              className="mt-2 logo-hyped1 "
            />
          </div>
          <div></div>
          <div className="dashboard-title mt-2">
            {' '}
            <Title>Dashboard</Title>
            <Text>Telemetric data stream from on device sensors.</Text>
          </div>
        </div>
        <div className="pt-[60px] flex flex-row-reverse gap-5 switch">
          <Switch
            onChange={switched}
            checked={clicked}
            offColor="#808080"
            onColor="#c91c10"
            onHandleColor="#FFFFFF"
            offHandleColor="#FFFFF"
            uncheckedIcon={false}
            checkedIcon={false}
            boxShadow="#c91c10"
            activeBoxShadow="#c91c10"
          />
          {clicked === false ? (
            <Badge size="xl">Light</Badge>
          ) : (
            <Badge size="xl" color="red">
              Dark
            </Badge>
          )}
        </div>
      </div>
      <div className="section1 flex row gap-3 w-[100%] h-[170px]">
        <DigitalTimer />
        <Gauge />
      </div>

      <div className="mt-5 top-card"> {CARDS[selected]}</div>
      {/* KPI section */}
      <div className="top-card">
        <Grid numItemsMd={2} className="mt-6 gap-6 w-full">
          {(Object.keys(CARDS) as Card[])
            .filter((c) => c !== selected)
            .map((c) => (
              <button key={c} onClick={() => setSelected(c)}>
                <div className="h-0" />
                {CARDS[c]}
              </button>
            ))}
        </Grid>
      </div>

      <Image
        alt="Hyped logo"
        src={clicked === false ? '/hypedLogoLight.png' : '/hypedLogoDark.png'}
        width="150"
        height="50"
        className="mt-5"
      />
      <SocialIcons />
    </main>
  );
}
