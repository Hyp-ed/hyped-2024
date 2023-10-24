import { useState, useEffect, createContext, useContext } from 'react';
import { socket } from '../lib/socket';

export type Log = {
  context: string;
  stack?: any;
  level: 'info' | 'warn' | 'error' | 'debug';
  message: string;
  timestamp: string;
};

type LiveLogsContext = {
  isConnected: boolean;
  logs: Log[];
};

const LiveLogsContext = createContext<LiveLogsContext | null>(null);

export const LiveLogsProvider = ({
  children,
}: {
  children: React.ReactNode;
}) => {
  const [isConnected, setIsConnected] = useState(false);
  const [logs, setLogs] = useState<Log[]>([]);

  useEffect(() => {
    function onConnect() {
      setIsConnected(true);
    }

    function onDisconnect() {
      setIsConnected(false);
    }

    function onLog(log: Log) {
      setLogs((logs) => [...logs, log]);
    }

    socket.on('connect', onConnect);
    socket.on('disconnect', onDisconnect);
    socket.on('log', onLog);

    return () => {
      socket.off('connect', onConnect);
      socket.off('disconnect', onDisconnect);
      socket.off('log', onLog);
    };
  }, []);

  return (
    <LiveLogsContext.Provider
      value={{
        isConnected,
        logs,
      }}
    >
      {children}
    </LiveLogsContext.Provider>
  );
};

export const useLiveLogs = () => {
  const context = useContext(LiveLogsContext);
  if (!context) {
    throw new Error('useLiveLogs must be used within LiveLogsProvider');
  }
  return context;
};
