#include "mqtt.hpp"
#include "mqtt_logger.hpp"

#include <chrono>

namespace hyped::core {
MqttLogger::MqttLogger(const char *const label,
                       const LogLevel level,
                       const core::ITimeSource &timer,
                       ILogger &logger,
                       std::shared_ptr<IMqtt> mqtt)
    : label_(label),
      level_(level),
      time_source_(time_source_),
      logger_(logger),
      mqtt_(mqtt)
{
}

void MqttLogger::log(const LogLevel level, const char *format, ...)
{
  logger_.log(level, format);
  const auto topic                                     = MqttTopic::kTest;
  std::shared_ptr<rapidjson::Document> message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();
  message_payload->AddMember("log", *format, message_payload->GetAllocator());
  const MqttMessage::Header header{.timestamp = 0, .priority = MqttMessagePriority::kCritical};
  const MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, MqttMessageQos::kAtLeastOnce);
}
}  // namespace hyped::core