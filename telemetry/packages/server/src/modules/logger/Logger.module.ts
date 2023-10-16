import { Module } from '@nestjs/common';
import { utilities, WinstonModule, WinstonModuleOptions } from 'nest-winston';
import { format, transports } from 'winston';
import { ENV } from '../core/config';

// In top-level 'telemetry' directory
const LOGGING_DIRECTORY = '../../logs';

const customFormat = format((info) => {
  if (info[Symbol.for('level')] === 'error' && info['error']) {
    return {
      context: info['error']['name'],
      message: `${info['error']['message']}`,
      // stack: info['error']['stack'],
      level: 'error',
      [Symbol.for('level')]: 'error',
    };
  }
  return info;
});

const loggerOptions: WinstonModuleOptions = {
  level: ENV === 'development' ? 'debug' : 'info',
  format: format.combine(
    customFormat(),
    format.timestamp({
      format: 'YYYY-MM-DD HH:mm:ss',
    }),
    format.errors({ stack: false }),
    format.splat(),
    format.json(),
  ),
  transports: [
    new transports.File({
      filename: 'errors.log',
      dirname: LOGGING_DIRECTORY,
      level: 'error',
    }),
    new transports.File({ filename: 'all.log', dirname: LOGGING_DIRECTORY }),
    ...(ENV === 'development'
      ? [
          new transports.Console({
            format: format.combine(
              format.timestamp(),
              format.ms(),
              utilities.format.nestLike('Telemetry', {
                colors: true,
                prettyPrint: true,
              }),
            ),
            handleExceptions: true,
          }),
        ]
      : []),
  ],
  exitOnError: false,
};

const logger = WinstonModule.forRoot(loggerOptions);

@Module({
  imports: [logger],
  exports: [logger],
})
export class LoggerModule {}
