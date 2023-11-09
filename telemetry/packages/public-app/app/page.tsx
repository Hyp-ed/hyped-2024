'use client';
import Image from 'next/image';

import { VelocityGraph } from '@/components/velocity-graph';
import { TrackerExample } from '@/components/tracker';
import GridDisplay from '@/components/gridDisplay';
import PreLoader from '@/components/preLoader';

// if (
//   localStorage.getItem('color-theme') === 'dark' ||
//   (!('color-theme' in localStorage) &&
//     window.matchMedia('(prefers-color-scheme: dark)').matches)
// ) {
//   document.documentElement.classList.add('dark');
// } else {
//   document.documentElement.classList.remove('dark');
// }

export default function Home() {
  return (
    <main className="min-h-screen">
      <PreLoader />
    </main>
  );
}
