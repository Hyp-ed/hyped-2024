import { LOG_LEVELS, Log, useLiveLogs } from '@/context/live-logs';
import { format } from 'date-fns';
import { useEffect } from 'react';

export const LogViewer = () => {
  const { isConnected, logs } = useLiveLogs();

  // Scroll to bottom on new log
  useEffect(() => {
    const element = document.getElementById('log-viewer');
    if (element) {
      element.scrollTop = element.scrollHeight;
    }
  }, [logs]);

  return (
    <div className="h-full p-12">
      <div className="h-full p-12 border-[1px] border-openmct-light-gray rounded-xl">
        {isConnected ? (
          <div
            id="log-viewer"
            className="h-full overflow-y-scroll scrollbar-track-transparent scrollbar-thumb-openmct-dark-gray scrollbar-thin scrollbar-thumb-rounded-full"
          >
            {logs.map((log, index) => (
              <SingleLog log={log} key={log.timestamp} />
            ))}
          </div>
        ) : (
          <div className="text-white text-center p-4">Connecting...</div>
        )}
      </div>
    </div>
  );
};

/**
 * A single log
 * @param log The log to display
 * @returns The log component
 */
const SingleLog = ({ log }: { log: Log }) => {
  const { context, message } = log;
  const colour = getLogColour(log.level);
  const level = log.level.replace(/\b\w/g, (l) => l.toUpperCase());
  const time = format(new Date(log.timestamp), 'yyyy-MM-dd HH:mm:ss');

  return (
    <p className="text-white text-sm font-logs flex gap-2">
      <span className="w-16 text-yellow-400">{level}</span>
      <span className="text-white">{time}</span>
      <span className="text-yellow-400">[{context}]</span>
      <span className={colour}>{message}</span>
    </p>
  );
};

/**
 * Get the colour of the log based on the level
 * @param level The level of the log
 * @returns The colour of the log
 */
const getLogColour = (level: string) => {
  switch (level) {
    case LOG_LEVELS.INFO:
      return 'text-green-500';
    case LOG_LEVELS.ERROR:
      return 'text-red-500';
    case LOG_LEVELS.WARN:
      return 'text-yellow-400';
    case LOG_LEVELS.DEBUG:
      return 'text-purple-400';
    default:
      return 'text-white';
  }
};
