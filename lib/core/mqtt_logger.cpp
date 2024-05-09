#include "mqtt.hpp"
#include "mqtt_logger.hpp"

#include <cstdarg>
#include <cstdio>
#include <utility>

namespace hyped::core {

MqttLogger::MqttLogger(const char *const label,
                       const LogLevel level,
                       const core::ITimeSource &timer,
                       ILogger &logger,
                       std::shared_ptr<IMqtt> mqtt)
    : label_(label),
      level_(level),
      time_source_(timer),
      logger_(logger),
      mqtt_(std::move(mqtt))
{
}

void MqttLogger::log(const LogLevel level, const char *format, ...)
{
  char buffer[256];  // NOLINT
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  logger_.log(level, "%s", buffer);
  const auto topic                                     = MqttTopic::kLog;
  std::shared_ptr<rapidjson::Document> message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();
  rapidjson::Value log_string_value;
  log_string_value.SetString(buffer, message_payload->GetAllocator());
  message_payload->AddMember("log", log_string_value, message_payload->GetAllocator());
  const auto now = static_cast<std::uint64_t>(time_source_.now().time_since_epoch().count());
  const MqttMessage::Header header{.timestamp = now, .priority = MqttMessagePriority::kCritical};
  const MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, MqttMessageQos::kAtLeastOnce);
}

}  // namespace hyped::core
