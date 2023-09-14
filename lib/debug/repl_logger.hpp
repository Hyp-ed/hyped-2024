#include "terminal.hpp"

#include <ncurses.h>

#include <core/logger.hpp>
#include <core/timer.hpp>

namespace hyped::debug {

class ReplLogger : public core::ILogger {
 public:
  ReplLogger(const char *label,
             const core::LogLevel level,
             const core::ITimeSource &time_source_,
             Terminal &terminal);
  void log(core::LogLevel level, const char *format, ...) override;

 private:
  void printHead(const char *title);
  const char *const label_;
  const core::LogLevel level_;
  const core::ITimeSource &time_source_;
  Terminal &terminal_;
};

}  // namespace hyped::debug