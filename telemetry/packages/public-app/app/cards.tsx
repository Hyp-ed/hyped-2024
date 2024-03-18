import { Card, Title, Text, Grid } from '@tremor/react';
import { VelocityGraph } from '@/components/velocity-graph';
import { useState } from 'react';
import { DisplacementChart } from '@/components/displacement-chart';
import Image from 'next/image';
import { LaunchTime } from '@/components/launch-time';
import LevitationHeight from '@/components/levitation-height';
import { SocialIcons } from '@/components/social-icons';

/**
 * The cards that are displayed on the dashboard.
 */
const CARDS = {
  VELOCITY: <VelocityGraph />,
  ACCELERATION: <DisplacementChart />,
  LEVITATION: <LevitationHeight />,
};

type Card = keyof typeof CARDS;

// TODO: reimplement dark theme

export default function Cards() {
  const [selected, setSelected] = useState<Card>('VELOCITY');

  return (
    <main className="m-4 py-12">
      <div className="nav p-2 mb-3">
        <div className="heading p-2">
          <div>
            <Image
              alt="Hyped logo"
              // TODO: make dynamic depending on the theme: https://nextjs.org/docs/app/api-reference/components/image#getimageprops
              src="/hypedLogoLight.png"
              width="200"
              height="50"
              className="mt-2 logo-hyped1 "
            />
          </div>
          <div></div>
          <div className="dashboard-title mt-2">
            <Title>Dashboard</Title>
            <Text>Telemetric data stream from on device sensors.</Text>
          </div>
        </div>
        {/* TODO: reimplement theme switch */}
      </div>
      <LaunchTime />
      <div className="mt-5 top-card"> {CARDS[selected]}</div>
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
        // TODO: make dynamic depending on the theme: https://nextjs.org/docs/app/api-reference/components/image#getimageprops
        src="/hypedLogoLight.png"
        width="150"
        height="50"
        className="mt-5"
      />
      <SocialIcons />
    </main>
  );
}
