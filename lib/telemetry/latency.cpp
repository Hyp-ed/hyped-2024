#include "latency.hpp"

#include <core/wall_clock.hpp>

namespace hyped::telemetry {

Latency::Latency(std::shared_ptr<core::IMqtt> mqtt)
{
  mqtt_->subscribe(core::MqttTopic::kStateRequest);
}

void Latency::respond()
{
  const auto nextMessage = mqtt_->getMessage();
  if (!nextMessage) { return; }

  const auto payload       = nextMessage->payload;
  const auto topic        = core::MqttTopic::kLatencyResponse;
  const core::MqttMessage::Header header{.timestamp = 0,
                                         .priority  = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message{topic, header, payload};
  mqtt_->publish(message, core::MqttMessageQos::kAtLeastOnce);
}

void Latency::run()
{
  mqtt_->consume();
  respond();
}

core::Result Latency::startNode(const std::string &mqtt_ip, const std::uint32_t mqtt_port)
{
  core::WallClock wall_clock;
  core::Logger logger("LATENCY", core::LogLevel::kDebug, wall_clock);
  auto optional_mqtt = core::Mqtt::create(logger, "latency", mqtt_ip, mqtt_port);
  if (!optional_mqtt) {
    logger.log(core::LogLevel::kFatal, "Failed to create MQTT client");
    return core::Result::kFailure;
  }
  auto mqtt = *optional_mqtt;
  Latency latency(mqtt);
  latency.run();
  return core::Result::kSuccess;
}

}  // namespace hyped::telemetry
