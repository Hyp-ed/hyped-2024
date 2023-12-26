#include "mqtt_logger.hpp"

#include <chrono>

namespace hyped::core {
MqttLogger::MqttLogger(const char *const label,
                       const LogLevel level,
                       const core::ITimeSource &timer,
                       std::shared_ptr<IMqtt> mqtt)
    : label_(label),
      level_(level),
      time_source_(time_source_),
      mqtt_(mqtt)
{
}

void MqttLogger::log(const LogLevel level, const char *format, ...)
{
  std::cout << "MqttLogger::log" << std::endl;
}
}  // namespace hyped::core