#include "mqtt.hpp"
#include "mqtt_logger.hpp"

#include <cstdarg>
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
  char buffer[256];
  va_list args;
  va_start(args, format);
  printf(format, args);
  snprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  const auto log_string = std::string(buffer);
  logger_.log(level, log_string.c_str());
  const auto topic                                     = MqttTopic::kTest;
  std::shared_ptr<rapidjson::Document> message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();
  rapidjson::Value log_string_value;
  log_string_value.SetString(log_string.c_str(), message_payload->GetAllocator());
  message_payload->AddMember("log", log_string_value, message_payload->GetAllocator());
  const MqttMessage::Header header{.timestamp = 0, .priority = MqttMessagePriority::kCritical};
  const MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, MqttMessageQos::kAtLeastOnce);
}

}  // namespace hyped::core
