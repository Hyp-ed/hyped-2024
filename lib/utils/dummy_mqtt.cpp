#include "dummy_mqtt.hpp"

namespace hyped::utils {

void DummyMqtt::publish(const core::MqttMessage &message, const core::MqttMessageQos qos)
{
  messages_sent_.push_back(message);
}

core::Result DummyMqtt::subscribe(const core::MqttTopic topic)
{
  return core::Result::kSuccess;
}

core::Result DummyMqtt::consume()
{
  return core::Result::kSuccess;
}

std::optional<core::MqttMessage> DummyMqtt::getMessage()
{
  if (messages_to_receive_.empty()) { return std::nullopt; }
  const auto next_message = messages_to_receive_.at(0);
  messages_to_receive_.erase(messages_to_receive_.begin());
  return next_message;
}

std::vector<core::MqttMessage> DummyMqtt::getSentMessages()
{
  return messages_sent_;
}

void DummyMqtt::addMessageToReceive(const core::MqttMessage &message)
{
  messages_to_receive_.emplace_back(message);
}

}  // namespace hyped::utils
