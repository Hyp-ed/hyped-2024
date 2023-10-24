import { useLiveLogs } from '@/context/live-logs';

export const LogViewer = () => {
  const { isConnected, logs } = useLiveLogs();

  return (
    <div className="bg-black rounded-xl shadow-xl h-32">
      {isConnected ? (
        <div className="p-4">
          {logs.map((log, index) => (
            <div key={index} className="text-white">
              {log.message}
            </div>
          ))}
        </div>
      ) : (
        <div className="text-white text-center p-4">Connecting...</div>
      )}
    </div>
  );
};
