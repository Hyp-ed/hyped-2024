'use client';
import Image from 'next/image';

import { VelocityGraph } from '@/components/velocity-graph';

import GridDisplay from '@/components/gridDisplay';
import PreLoader from '@/components/preLoader';


export default function Home() {
  return (
    <main className="min-h-screen">
      <PreLoader />
    </main>
  );
}
