#pragma once

#include <string>
#include <unordered_map>

namespace hyped::core {

enum class MqttTopic { kTest, kState, kAccelerometer, kOpticalFlow, kKeyence};

const std::unordered_map<MqttTopic, std::string> mqtt_topic_to_string
  = {{MqttTopic::kTest, "test"}, {MqttTopic::kState, "state"}, {MqttTopic::kAccelerometer, "accelerometer"}, {MqttTopic::kOpticalFlow, "optical_flow"}, {MqttTopic::kKeyence, "keyence"}};
const std::unordered_map<std::string, MqttTopic> mqtt_string_to_topic
  = {{"test", MqttTopic::kTest}, {"state", MqttTopic::kState}, {"accelerometer", MqttTopic::kAccelerometer}, {"optical_flow", MqttTopic::kOpticalFlow}, {"keyence", MqttTopic::kKeyence}};

}  // namespace hyped::core