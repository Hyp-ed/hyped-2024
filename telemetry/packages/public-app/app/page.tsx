'use client';
import Image from 'next/image'
import KpiCard from '../components/test'
import { VelocityGraph } from '@/components/velocity-graph'
import { TrackerExample } from '@/components/tracker'
import GridDisplay from '@/components/gridDisplay'
import PreLoader from '@/components/preLoader'


export default function Home() {
  
  return (
    <main className="flex min-h-screen flex-col items-center justify-between p-10 gap-10 ">
      <PreLoader/>
      
    </main>
  )
}
