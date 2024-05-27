#pragma once

#include <string>
#include <unordered_map>

namespace hyped::core {

enum class MqttTopic { kTest, kState, kLog, kNodeCheckin, kHeartbeat, kStateRequest };

const std::unordered_map<MqttTopic, std::string> mqtt_topic_to_string
  = {{MqttTopic::kTest, "test"},
     {MqttTopic::kState, "state"},
     {MqttTopic::kLog, "log"},
     {MqttTopic::kNodeCheckin, "node_checkin"},
     {MqttTopic::kStateRequest, "state_request"}};

const std::unordered_map<std::string, MqttTopic> mqtt_string_to_topic
  = {{"test", MqttTopic::kTest},
     {"state", MqttTopic::kState},
     {"log", MqttTopic::kLog},
     {"node_checkin", MqttTopic::kNodeCheckin},
     {"state_request", MqttTopic::kStateRequest}};

}  // namespace hyped::core
