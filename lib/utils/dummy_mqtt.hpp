#pragma once

#include <core/mqtt.hpp>

namespace hyped::utils {

class DummyMqtt : public core::IMqtt {
 public:
  DummyMqtt();
  ~DummyMqtt();
  void publish(const core::MqttMessage &message, const core::MqttMessageQos qos);
  core::Result subscribe(const core::MqttTopic topic);
  core::Result consume();
  std::optional<core::MqttMessage> getMessage();

  std::vector<core::MqttMessage> getSentMessages();
  void addMessageToReceive(const core::MqttMessage &message);

 private:
  std::vector<std::optional<core::MqttMessage>> messages_to_receive_;
  std::vector<core::MqttMessage> messages_sent_;
};

}  // namespace hyped::utils
