#pragma once

#include <core/logger.hpp>

namespace hyped::utils {

class DummyLogger : public core::ILogger {
 public:
  DummyLogger() = default;
  void log(const core::LogLevel level, const char *format...) override;
};

}  // namespace hyped::utils
