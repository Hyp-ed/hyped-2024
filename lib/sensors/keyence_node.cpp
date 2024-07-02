#include "keyence.hpp"
#include "keyence_node.hpp"

#include <utility>

#include "core/logger.hpp"
#include "core/mqtt.hpp"
#include "core/wall_clock.hpp"
#include "io/hardware_gpio.hpp"

namespace hyped::sensors {

core::Result KeyenceNode::startNode(toml::v3::node_view<const toml::v3::node> config,
                                    const std::string &mqtt_ip,
                                    const std::uint32_t mqtt_port)
{
  auto time          = core::WallClock();
  auto logger        = core::Logger("Keyence", core::LogLevel::kDebug, time);
  auto optional_mqtt = core::Mqtt::create(logger, "Keyence", mqtt_ip, mqtt_port);
  if (!optional_mqtt) {
    logger.log(core::LogLevel::kFatal, "Failed to create MQTT client");
    return core::Result::kFailure;
  }
  auto mqtt      = *optional_mqtt;
  auto gpio      = std::make_shared<io::HardwareGpio>(logger);
  const auto pin = config["pin"].value<std::uint8_t>();
  if (!pin) {
    logger.log(core::LogLevel::kFatal, "No pin specified");
    return core::Result::kFailure;
  }
  auto optional_keyence = Keyence::create(logger, gpio, *pin);
  if (!optional_keyence) {
    logger.log(core::LogLevel::kFatal, "Failed to create keyence");
    return core::Result::kFailure;
  }
  auto keyence = *optional_keyence;
  auto node    = KeyenceNode(logger, time, mqtt, keyence);
  node.run();
  return core::Result::kSuccess;
}

KeyenceNode::KeyenceNode(core::Logger &logger,
                         core::ITimeSource &time,
                         std::shared_ptr<core::Mqtt> mqtt,
                         std::shared_ptr<Keyence> keyence)
    : logger_(logger),
      time_(time),
      mqtt_(std::move(mqtt)),
      keyence_(std::move(keyence))
{
}

void KeyenceNode::requestFailure()
{
  std::string failure_message = "kFailure";
  const auto topic            = core::MqttTopic::kState;
  auto message_payload        = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();

  rapidjson::Value message_value;
  message_value.SetString(failure_message.c_str(), message_payload->GetAllocator());
  message_payload->AddMember("state", message_value, message_payload->GetAllocator());

  const core::MqttMessage::Header header{
    .timestamp = static_cast<std::uint64_t>(time_.now().time_since_epoch().count()),
    .priority  = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
}

void KeyenceNode::run()
{
  while (true) {
    auto result = keyence_->updateStripeCount();
    if (result == core::Result::kFailure) {
      requestFailure();
      return;
    }
    const auto stripe_count = keyence_->getStripeCount();
    const auto topic        = core::MqttTopic::kKeyence;
    auto message_payload    = std::make_shared<rapidjson::Document>();
    message_payload->SetObject();
    rapidjson::Value stripe_count_json(stripe_count);
    message_payload->AddMember("count", stripe_count_json, message_payload->GetAllocator());
    const core::MqttMessage::Header header{
      .timestamp = static_cast<std::uint64_t>(time_.now().time_since_epoch().count()),
      .priority  = core::MqttMessagePriority::kNormal};
    const core::MqttMessage message{topic, header, message_payload};
    mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
  }
}

}  // namespace hyped::sensors
