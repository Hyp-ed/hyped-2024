#include "logger.hpp"

#include <chrono>

namespace hyped::core {

Logger::Logger(const char *const label, const LogLevel level, const core::ITimeSource &time_source_)
    : label_(label),
      level_(level),
      time_source_(time_source_)
{
}

void Logger::printHead(FILE *file, const char *title)
{
  const auto time_point         = time_source_.now();
  const auto timestamp          = std::chrono::system_clock::to_time_t(time_point);
  const auto time_point_seconds = std::chrono::system_clock::from_time_t(timestamp);
  const std::chrono::milliseconds time_point_milliseconds
    = std::chrono::duration_cast<std::chrono::milliseconds>(time_point - time_point_seconds);
  const long long time_milliseconds = time_point_milliseconds.count();
  const std::tm *time_struct        = localtime(&timestamp);
  fprintf(file,
          "%02d:%02d:%02d.%03lld %s[%s] ",
          time_struct->tm_hour,
          time_struct->tm_min,
          time_struct->tm_sec,
          time_milliseconds,
          title,
          label_);
}

void Logger::log(const LogLevel level, const char *format, ...)
{
  FILE *file;
  if (level_ <= level && level_ != LogLevel::kNone) {
    switch (level) {
      case LogLevel::kDebug:
        file = stdout;
        printHead(file, "DEBUG");
        break;
      case LogLevel::kInfo:
        file = stdout;
        printHead(file, "INFO");
        break;
      case LogLevel::kWarn:
        file = stderr;
        printHead(file, "WARN");
        break;
      case LogLevel::kFatal:
        file = stderr;
        printHead(file, "FATAL");
        break;
      default:
        break;
    }
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    fprintf(file, "\n");
    va_end(args);
  }
}

}  // namespace hyped::core
