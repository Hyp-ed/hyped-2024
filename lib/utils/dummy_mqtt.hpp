#pragma once

#include <core/mqtt.hpp>

namespace hyped::utils {

class DummyMqtt : public core::IMqtt {
 public:
  DummyMqtt();
  ~DummyMqtt();
  void publish(const core::MqttMessage &message, const core::MqttMessageQos qos) override;
  core::Result subscribe(const core::MqttTopic topic) override;
  core::Result consume() override;
  std::optional<core::MqttMessage> getMessage() override;

  std::vector<core::MqttMessage> getSentMessages();
  void addMessageToReceive(const core::MqttMessage &message);

 private:
  std::vector<std::optional<core::MqttMessage>> messages_to_receive_;
  std::vector<core::MqttMessage> messages_sent_;
};

}  // namespace hyped::utils
