#include "dummy_mqtt.hpp"

namespace hyped::utils {

void MockMqtt::publish(const core::MqttMessage &message, const core::MqttMessageQos qos)
{
}

core::Result MockMqtt::subscribe(const core::MqttTopic topic)
{
  return core::Result::kSuccess;
}

core::Result MockMqtt::consume()
{
  return core::Result::kSuccess;
}

std::optional<core::MqttMessage> MockMqtt::getMessage()
{
  return std::nullopt;
}

}  // namespace hyped::utils
