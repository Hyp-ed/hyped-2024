#pragma once

#include <core/logger.hpp>

namespace hyped::utils {

class DummyLogger : public core::ILogger {
 public:
  DummyLogger();
  virtual void log(const core::LogLevel level, const char *format...);
};

}  // namespace hyped::utils
