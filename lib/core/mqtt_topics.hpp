#pragma once

#include <string>
#include <unordered_map>

namespace hyped::core {

enum class MqttTopic {
  kState,
  kStateRequest,
  kAccelerometer,
  kOpticalFlow,
  kKeyence,
  kStarted,
  kDisplacement,
  kVelocity,
  kAcceleration,
};

const std::unordered_map<MqttTopic, std::string> mqtt_topic_to_string
  = {{MqttTopic::kState, "hyped/cart_2024/state/state"},
     {MqttTopic::kStateRequest, "hypd/cart_2024/state/state_request"},
     {MqttTopic::kAccelerometer, "hyped/cart_2024/measurement/accelerometer"},
     {MqttTopic::kOpticalFlow, "hypd/cart_2024/measurement/optical_flow"},
     {MqttTopic::kKeyence, "hyped/cart_2024/measurement/keyence"},
     {MqttTopic::kDisplacement, "hypd/cart_2024/navigation/displacement"},
     {MqttTopic::kVelocity, "hypd/cart_2024/navigation/velocity"},
     {MqttTopic::kAcceleration, "hypd/cart_2024/navigation/acceleration"}};

const std::unordered_map<std::string, MqttTopic> mqtt_string_to_topic
  = {{"hyped/cart_2024/state/state", MqttTopic::kState},
     {"hypd/cart_2024/state/state_request", MqttTopic::kState},
     {"hyped/cart_2024/measurement/accelerometer", MqttTopic::kAccelerometer},
     {"hyped/cart_2024/measurement/optical_flow", MqttTopic::kOpticalFlow},
     {"hyped/cart_2024/measurement/keyence", MqttTopic::kKeyence},
     {"hypd/cart_2024/navigation/displacement", MqttTopic::kDisplacement},
     {"hypd/cart_2024/navigation/velocity", MqttTopic::kVelocity},
     {"hypd/cart_2024/navigation/acceleration", MqttTopic::kAcceleration}};

}  // namespace hyped::core
