#pragma once

#include "time.hpp"

#include <cstdarg>
#include <iostream>
#include <optional>

#define logNTimes(logger, n, level, ...)                                                           \
  do {                                                                                             \
    static std::size_t count = 0;                                                                  \
    if (count < n) {                                                                               \
      logger.log(level, __VA_ARGS__);                                                              \
      ++count;                                                                                     \
    }                                                                                              \
  } while (0)

#define logEveryNth(logger, n, level, ...)                                                         \
  do {                                                                                             \
    static std::size_t count = n;                                                                  \
    if (count == n) {                                                                              \
      logger.log(level, __VA_ARGS__);                                                              \
      count = 0;                                                                                   \
    }                                                                                              \
    ++count;                                                                                       \
  } while (0)

namespace hyped::core {

/**
 * kNone: no logging
 * kDebug: logging debug info (successess, values read, etc.)
 * kInfo: logging of important events (state changes, etc.)
 * kWarn: logging non-fatal unexpected events that do not stop the program
 * kFatal: logging fatal unexpected events that stop the program
 */
enum class LogLevel { kNone = 0, kDebug, kInfo, kWarn, kFatal };

class ILogger {
 public:
  virtual void log(const LogLevel level, const char *format...) = 0;
};

class Logger : public ILogger {
 public:
  Logger(const char *const label, const LogLevel level, const core::ITimeSource &timer);

  void log(const LogLevel level, const char *format, ...);

 private:
  void printHead(FILE *file, const char *title);
  const char *const label_;
  const LogLevel level_;
  const core::ITimeSource &time_source_;
};
}  // namespace hyped::core
