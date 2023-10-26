import { useLiveLogs } from '@/context/live-logs';
import { cn } from '@/lib/utils';
import { format } from 'date-fns';
import { TextCursor } from 'lucide-react';
import { useEffect } from 'react';

export const LogViewer = () => {
  const { isConnected, logs } = useLiveLogs();

  // scroll to bottom on new log
  useEffect(() => {
    const element = document.getElementById('log-viewer');
    if (element) {
      element.scrollTop = element.scrollHeight;
    }
  }, [logs]);

  return (
    <div className="h-full p-8">
      {isConnected ? (
        <div
          id="log-viewer"
          className="max-h-[88vh] overflow-y-scroll scrollbar-track-transparent scrollbar-thumb-openmct-dark-gray scrollbar-thin scrollbar-thumb-rounded-full"
        >
          {logs.map((log, index) => {
            let colour = 'text-white';
            switch (log.level) {
              case 'info':
                colour = 'text-green-500';
                break;
              case 'error':
                colour = 'text-red-500';
                break;
              case 'warn':
                colour = 'text-yellow-400';
                break;
              case 'debug':
                colour = 'text-purple-400';
                break;
            }

            return (
              <p
                key={index}
                className="text-white text-sm font-logs flex gap-2"
              >
                <span className="w-16 text-yellow-400">
                  {log.level.replace(/\b\w/g, (l) => l.toUpperCase())}
                </span>
                <span className="text-white">
                  {format(new Date(log.timestamp), 'yyyy-MM-dd HH:mm:ss')}
                </span>
                <span className="text-yellow-400">[{log.context}]</span>
                <span className={colour}>{log.message}</span>
              </p>
            );
          })}
        </div>
      ) : (
        <div className="text-white text-center p-4">Connecting...</div>
      )}
    </div>
  );
};
