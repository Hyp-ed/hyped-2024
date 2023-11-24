import React from 'react';
import '../app/globals.css';
import { useEffect, useState } from 'react';
import GridDisplay from './gridDisplay';
import Image from 'next/image';

const style: React.CSSProperties = {
  backgroundColor: 'black',
  color: 'white',
  fontSize: '20px',
  justifyContent: 'center',
  alignContent: 'center',
  alignItems: 'center', // corrected property name from alignContent to alignItems
  display: 'flex',
  flexDirection: 'column', // added display property to center content horizontally
  height: '100vh', // added height to center content vertically
  width: '100vw',
  // margin: 'auto',
};

export default function PreLoader(): JSX.Element {
  //const [data, setData] = useState([])
  const [loading, setLoading] = useState<boolean>(true);
  const [processing, setProcessing] = useState<boolean>(true);
  const [processed, setProcessed] = useState<boolean>(true);

  useEffect(() => {
    setTimeout(() => {
      setProcessing(false);
    }, 0);
    setTimeout(() => {
      setProcessed(false);
    }, 0);
  }, []);

  useEffect(() => {
    setTimeout(() => {
      setLoading(false);
    }, 0);
  }, []);

  return (
    <div>
      {loading ? (
        <div>
          <div style={style} className="fade-in-image">
            <Image
              alt="hyped logo"
              src="/hyped.svg"
              className="pb-10"
              width={100}
              height={100}
            />
            <div className="processing">
              {processing ? (
                <h1>Authenticating...</h1>
              ) : processed ? (
                <h1>Performing Security Checks...</h1>
              ) : (
                <h1>Granting Access...</h1>
              )}
            </div>
          </div>
        </div>
      ) : (
        <GridDisplay />
      )}
    </div>
  );
}
