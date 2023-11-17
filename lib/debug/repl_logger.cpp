#include "repl_logger.hpp"

#include <ncurses.h>

#include <chrono>

namespace hyped::debug {

ReplLogger::ReplLogger(const char *const label,
                       const core::LogLevel level,
                       const core::ITimeSource &time_source_,
                       Terminal &terminal)
    : label_(label),
      level_(level),
      time_source_(time_source_),
      terminal_(terminal)
{
}

void ReplLogger::printHead(const char *title)
{
  const auto time_point         = time_source_.now();
  const auto timestamp          = std::chrono::system_clock::to_time_t(time_point);
  const auto time_point_seconds = std::chrono::system_clock::from_time_t(timestamp);
  const std::chrono::milliseconds time_point_milliseconds
    = std::chrono::duration_cast<std::chrono::milliseconds>(time_point - time_point_seconds);
  const long long time_milliseconds = time_point_milliseconds.count();
  const std::tm *time_struct        = localtime(&timestamp);
  terminal_.printf("%02d:%02d:%02d.%03lld %s[%s] ",
                   time_struct->tm_hour,
                   time_struct->tm_min,
                   time_struct->tm_sec,
                   time_milliseconds,
                   title,
                   label_);
}

void ReplLogger::log(const core::LogLevel level, const char *format, ...)
{
  if (level_ <= level && level_ != core::LogLevel::kNone) {
    switch (level) {
      case core::LogLevel::kDebug:
        printHead("DEBUG");
        break;
      case core::LogLevel::kInfo:
        printHead("INFO");
        break;
      case core::LogLevel::kWarn:
        printHead("WARN");
        break;
      case core::LogLevel::kFatal:
        printHead("FATAL");
        break;
      default:
        break;
    }
    // Create string from variadic arguments
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    terminal_.println(buffer);
  }
}

}  // namespace hyped::debug