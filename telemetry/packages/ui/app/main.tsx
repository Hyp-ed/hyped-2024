import * as React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App';
import './globals.css';
import 'victormono';
import '@fontsource/raleway';
import { Toaster } from 'react-hot-toast';
import { MQTTProvider } from './context/mqtt';
import { PodsProvider } from './context/pods';
import { POD_IDS } from '@hyped/telemetry-constants';

const MQTT_BROKER = 'ws://localhost:8080';
const QOS = 0;

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
  // @ts-ignore
  <React.StrictMode>
    <MQTTProvider broker={MQTT_BROKER} qos={QOS}>
      <PodsProvider podIds={POD_IDS as unknown as string[]}>
        <App />
        <Toaster
          position="bottom-center"
          reverseOrder={false}
          toastOptions={{
            className: 'bg-gray-100 text-gray-900 shadow-xl',
          }}
        />
      </PodsProvider>
    </MQTTProvider>
  </React.StrictMode>,
);
