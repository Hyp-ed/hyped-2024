import React, { useState, useEffect } from 'react';
import { useQuery } from 'react-query';
import { Chart } from 'react-google-charts';
import { Card } from '@tremor/react';

function getRandomNumber() {
  return Math.random() * 100;
}

export function getData() {
  return [
    ['Label', 'Value'],
    ['Pressure', getRandomNumber()],
    ['Acceleration', getRandomNumber()],
    ['Thermometer', getRandomNumber()],
  ];
}

export function Gauge() {
  const [data1, setData] = useState(getData);
  const [widthX, setWidth] = useState<number | undefined>(
    window.innerWidth < 770 ? 300 : 400,
  );

  const { data, error } = useQuery<any>(
    'Accelerometer',
    async () =>
      await fetch(
        `${process.env.NEXT_PUBLIC_TELEMETRY_SERVER}/pods/pod_1/public-data/'acceleration'?start=0`,
      ).then((res) => res.json()),
    {
      refetchInterval: 1000,
    },
  );

  const options = {
    width: widthX,
    height: 120,
    redFrom: 90,
    redTo: 100,
    yellowFrom: 75,
    yellowTo: 90,
    minorTicks: 5,
  };

  useEffect(() => {
    const setScreenSize = () => {
      if (window.innerWidth < 770) {
        setWidth(300);
      } else {
        setWidth(400);
      }
    };

    window.addEventListener('resize', setScreenSize);
    return () => {
      window.removeEventListener('resize', setScreenSize);
    };
  });

  useEffect(() => {
    const id = setInterval(() => {
      setData(getData());
    }, 2000);

    return () => {
      clearInterval(id);
    };
  });

  return (
    <Card
      decoration="top"
      decorationColor="red"
      className=" pl-[6%] w-[50%] gauge-card"
    >
      <div className="gauge-div ">
        <Chart
          chartType="Gauge"
          // width={widthX}
          height="400px"
          data={data1}
          options={options}
        />
      </div>
    </Card>
  );
}
