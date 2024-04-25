#pragma once

#include "logger.hpp"
#include "mqtt.hpp"
#include "time.hpp"

namespace hyped::core {
class MqttLogger : public ILogger {
 public:
  MqttLogger(const char *const label,
             const LogLevel level,
             const core::ITimeSource &timer,
             ILogger &logger,
             std::shared_ptr<IMqtt> mqtt);
  void log(const LogLevel level, const char *format, ...);

 private:
  const char *const label_;
  const LogLevel level_;
  const core::ITimeSource &time_source_;
  ILogger &logger_;
  std::shared_ptr<IMqtt> mqtt_;
};
}  // namespace hyped::core