import { Card, Title, Text, Grid } from "@tremor/react";
import { VelocityGraph } from "./velocity-graph";
import KpiCard from "./test";
import { TrackerExample } from "./tracker";
import { MouseEventHandler } from "react";

const cards= [<VelocityGraph/>,< KpiCard/>, <TrackerExample/>]

export const inFocus=(event: MouseEventHandler<HTMLButtonElement>) => {
  const targetElement = event;
  const classNames = targetElement.
  let i:number
  if (classNames === 'v-graph'){
    i = 1
  } else if (classNames === 'tracker'){
     i = 3
  } else {
     i = 2
  }
    
}

export default function GridDsiplay() {
  const ColorInverter= (): void =>{
    
    document.documentElement.style.setProperty('--bg-color', (document.documentElement.style.getPropertyValue('--bg-color')==='black')?('white'):('black'))
    document.documentElement.style.setProperty('--invert-color', (document.documentElement.style.getPropertyValue('--bg-color')==='black')?('white'):('black'))
  }
  let x:number
   
  let y:number
  var i = 1
  x=2
  y=3
  
y = 3
  if (i=1) {x=2, y= 3} 
  if (i=2) {x=1, y=3} 
  if (i=3) {x=1, y=3}

  return (
    <main className="invert-mode">
      <Title className="invert-mode" >Dashboard</Title>
      <Text className="invert-mode" >Lorem ipsum dolor sit amet, consetetur sadipscing elitr.</Text>
      <button  className='invert-mode' onClick ={ColorInverter} >press</button>

      {/* Main section */}
      <Card className="mt-6 sm:w-[370px] md:w-[640px] lg:w-[900px]">
        <div className="h-0" />
        {cards[i]}
      </Card>

      {/* KPI section */}
      <Grid numItemsMd={2} className="mt-6 gap-6">
        <Card>
          {/* Placeholder to set height */}
          <div className="h-0" />
          {cards[x]}
        </Card>
        <Card>
          {/* Placeholder to set height */}
          <div className="h-0" />
          {cards[y]}
        </Card>
      </Grid>
    </main>
  );
}