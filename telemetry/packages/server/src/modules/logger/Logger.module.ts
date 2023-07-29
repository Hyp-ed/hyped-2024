import { Module } from '@nestjs/common';
import { utilities, WinstonModule, WinstonModuleOptions } from 'nest-winston';
import path from 'path';
import { format, transports } from 'winston';
import { ENV } from '../core/config';

// Same level as src
const LOGGING_DIRECTORY = path.join(__dirname, '..', '..', 'logs');

const loggerOptions: WinstonModuleOptions = {
  level: ENV === 'development' ? 'debug' : 'info',
  format: format.combine(
    format.timestamp({
      format: 'YYYY-MM-DD HH:mm:ss',
    }),
    format.errors({ stack: true }),
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
          }),
        ]
      : []),
  ],
};

const logger = WinstonModule.forRoot(loggerOptions);

@Module({
  imports: [logger],
  exports: [logger],
})
export class LoggerModule {}
