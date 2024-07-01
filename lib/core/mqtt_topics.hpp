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
  kLogs
};

const std::unordered_map<MqttTopic, std::string> mqtt_topic_to_string
  = {{MqttTopic::kState, "hyped/cart_2024/state/state"},
     {MqttTopic::kStateRequest, "hyped/cart_2024/state/state_request"},
     {MqttTopic::kAccelerometer, "hyped/cart_2024/measurement/accelerometer"},
     {MqttTopic::kOpticalFlow, "hyped/cart_2024/measurement/optical_flow"},
     {MqttTopic::kKeyence, "hyped/cart_2024/measurement/keyence"},
     {MqttTopic::kDisplacement, "hyped/cart_2024/navigation/displacement"},
     {MqttTopic::kVelocity, "hyped/cart_2024/navigation/velocity"},
     {MqttTopic::kAcceleration, "hyped/cart_2024/navigation/acceleration"},
     {MqttTopic::kLogs, "hyped/cart_2024/logs"}};

const std::unordered_map<std::string, MqttTopic> mqtt_string_to_topic
  = {{"hyped/cart_2024/state/state", MqttTopic::kState},
     {"hyped/cart_2024/state/state_request", MqttTopic::kState},
     {"hyped/cart_2024/measurement/accelerometer", MqttTopic::kAccelerometer},
     {"hyped/cart_2024/measurement/optical_flow", MqttTopic::kOpticalFlow},
     {"hyped/cart_2024/measurement/keyence", MqttTopic::kKeyence},
     {"hyped/cart_2024/navigation/displacement", MqttTopic::kDisplacement},
     {"hyped/cart_2024/navigation/velocity", MqttTopic::kVelocity},
     {"hyped/cart_2024/navigation/acceleration", MqttTopic::kAcceleration},
     {"hyped/cart_2024/logs", MqttTopic::kLogs}};

}  // namespace hyped::core
