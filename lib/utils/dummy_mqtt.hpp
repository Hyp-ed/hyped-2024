#pragma once

#include <core/mqtt.hpp>

namespace hyped::utils {

class MockMqtt : public core::IMqtt {
 public:
  MockMqtt();
  ~MockMqtt();
  void publish(const core::MqttMessage &message, const core::MqttMessageQos qos);
  core::Result subscribe(const core::MqttTopic topic);
  core::Result consume();
  std::optional<core::MqttMessage> getMessage();
};

}  // namespace hyped::utils