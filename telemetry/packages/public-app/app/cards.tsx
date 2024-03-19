import { Card, Title, Text, Grid } from '@tremor/react';
import { VelocityGraph } from '@/components/velocity-graph';
import { useState } from 'react';
import { DisplacementChart } from '@/components/displacement-chart';
import Image, { getImageProps } from 'next/image';
import { LaunchTime } from '@/components/launch-time';
import LevitationHeight from '@/components/levitation-height';
import { SocialIcons } from '@/components/social-icons';
import ThemeSwitch from '@/components/theme-switch';

/**
 * The cards that are displayed on the dashboard.
 */
const CARDS = {
  VELOCITY: <VelocityGraph />,
  ACCELERATION: <DisplacementChart />,
  LEVITATION: <LevitationHeight />,
};

type Card = keyof typeof CARDS;

export default function Cards() {
  const [selected, setSelected] = useState<Card>('VELOCITY');

  const selectedCardComponent = CARDS[selected];
  const otherCards = (Object.keys(CARDS) as Card[]).filter(
    (c) => c !== selected,
  );

  return (
    <div className="gap-4 sm:max-w-2xl sm:mx-auto flex-1 flex flex-col">
      <div className="flex flex-col items-center pb-4">
        <HypedImage />
      </div>
      <div className="space-y-0">
        <Title>Dashboard</Title>
        <Text>Telemetric data stream from on device sensors.</Text>
      </div>
      <LaunchTime />
      {selectedCardComponent}
      <Grid numItemsMd={2} className="gap-4 w-full">
        {otherCards.map((c) => (
          <button key={c} onClick={() => setSelected(c)}>
            <div className="h-0" />
            {CARDS[c]}
          </button>
        ))}
      </Grid>
      <div className="flex justify-center flex-col items-center gap-4 sm:gap-8 pt-8">
        <div className="text-black dark:text-white flex gap-4">
          Theme: <ThemeSwitch />
        </div>
        <HypedImage />
        <SocialIcons />
      </div>
    </div>
  );
}

/**
 * The HYPED logo image - changes depending on the theme.
 * @returns The HYPED logo as an image.
 */
const HypedImage = () => {
  const common = {
    alt: 'HYPED Logo, with a red E resembling 3 stacked hyperloop pods',
    width: 200,
    height: 50,
  };

  return (
    <>
      <Image
        {...common}
        alt={common.alt}
        src="/hyped-light.png"
        className="dark:hidden"
      />
      <Image
        {...common}
        alt={common.alt}
        src="/hyped-dark.png"
        className="hidden dark:block"
      />
    </>
  );
};
