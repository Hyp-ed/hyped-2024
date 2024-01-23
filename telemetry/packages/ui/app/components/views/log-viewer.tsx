import { LOG_LEVELS, Log, LogLevel, useLiveLogs } from '@/context/live-logs';
import { format } from 'date-fns';
import { useEffect, useState } from 'react';
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu';
import { Button } from '../ui/button';
import { ChevronDown } from 'lucide-react';

export const LogViewer = () => {
  const { isConnected, logs } = useLiveLogs();
  const [logLevelFilters, setLogLevelFilters] = useState<LogLevel[]>([
    LOG_LEVELS.INFO,
    LOG_LEVELS.ERROR,
    LOG_LEVELS.WARN,
    LOG_LEVELS.DEBUG,
    LOG_LEVELS.VERBOSE,
  ]);

  // Scroll to bottom on new log
  useEffect(() => {
    const element = document.getElementById('log-viewer');
    if (element) {
      element.scrollTop = element.scrollHeight;
    }
  }, [logs]);

  /**
   * Filter the logs based on the log level filters...
   */
  const displayLogs = logs.filter((log) => logLevelFilters.includes(log.level));

  return (
    <div className="h-full p-16 space-y-8">
      <div className="flex justify-between items-center">
        <div className="flex gap-2 items-center">
          {isConnected ? (
            <>
              <div className="w-2 h-2 rounded-full bg-green-500 animate-[pulse_linear_1s_infinite]" />
              <p className="italic text-green-500">Connected</p>
            </>
          ) : (
            <>
              <div className="w-2 h-2 rounded-full bg-red-500" />
              <p className="italic text-red-500">Disconnected</p>
            </>
          )}
        </div>
        <LogLevelFilter
          logLevelFilters={logLevelFilters}
          setLogLevelFilters={setLogLevelFilters}
        />
      </div>
      {isConnected ? (
        <div
          id="log-viewer"
          className="h-[90%] overflow-y-scroll scrollbar-track-transparent scrollbar-thumb-openmct-dark-gray scrollbar-thin scrollbar-thumb-rounded-full"
        >
          {displayLogs.map((log, index) => (
            <SingleLog log={log} key={index} />
          ))}
        </div>
      ) : (
        <div className="text-white text-center p-4">Connecting...</div>
      )}
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

const LogLevelFilter = ({
  logLevelFilters,
  setLogLevelFilters,
}: {
  logLevelFilters: LogLevel[];
  setLogLevelFilters: (logLevelFilters: LogLevel[]) => void;
}) => (
  <DropdownMenu>
    <DropdownMenuTrigger asChild>
      <Button variant="outline" className="flex gap-2">
        Log levels
        <ChevronDown className="h-4 w-4 opacity-50" />
      </Button>
    </DropdownMenuTrigger>
    <DropdownMenuContent align="end">
      {Object.values(LOG_LEVELS).map((logLevel) => (
        <DropdownMenuCheckboxItem
          key={logLevel}
          className="capitalize"
          checked={logLevelFilters.includes(logLevel)}
          onCheckedChange={(value) => {
            if (value) {
              setLogLevelFilters([...logLevelFilters, logLevel]);
            } else {
              setLogLevelFilters(
                logLevelFilters.filter((filter) => filter !== logLevel),
              );
            }
          }}
        >
          {logLevel}
        </DropdownMenuCheckboxItem>
      ))}
    </DropdownMenuContent>
  </DropdownMenu>
);

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
