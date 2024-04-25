#pragma once
#include "mqtt_topics.hpp"

#include <cstdint>
#include <optional>

#include <state_machine/state.hpp>

namespace hyped::core {

struct StateMessage {
  state_machine::State state;
};

struct NavigationMessage {};

// all messages with a higher priority are processed before any lower priority message is processed
enum MqttMessagePriority {
  kCritical = 0,
  kNormal   = 1,
  kLow      = 2,
};

enum MqttMessageQos {
  kAtMostOnce  = 0,
  kAtLeastOnce = 1,
  kExactlyOnce = 2,
};

struct MqttMessage {
  MqttTopic topic;
  struct Header {
    std::uint64_t timestamp;
    MqttMessagePriority priority;
  };
  Header header;
  std::optional<StateMessage> state_message;
  std::optional<NavigationMessage> navigation_message;
  bool operator<(const MqttMessage &other) const
  {
    if (header.priority == other.header.priority) {
      return header.timestamp > other.header.timestamp;
    }
    return header.priority < other.header.priority;
  }
  MqttMessage(const MqttTopic topic, const Header &header, const StateMessage &payload)
      : topic(topic),
        header(header),
        state_message(payload),
        navigation_message(std::nullopt)
  {
  }

  MqttMessage(const MqttTopic topic, const Header &header, const NavigationMessage &payload)
      : topic(topic),
        header(header),
        state_message(std::nullopt),
        navigation_message(payload)
  {
  }
};
}  // namespace hyped::core
