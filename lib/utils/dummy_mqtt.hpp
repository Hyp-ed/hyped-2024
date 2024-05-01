#pragma once

#include <core/mqtt.hpp>

namespace hyped::utils {

class MockMqtt : public core::IMqtt {
 public:
  MockMqtt()  = default;
  ~MockMqtt() = default;
  void publish(const core::MqttMessage &message, const core::MqttMessageQos qos) override;
  core::Result subscribe(const core::MqttTopic topic) override;
  core::Result consume() override;
  std::optional<core::MqttMessage> getMessage() override;
};

}  // namespace hyped::utils
