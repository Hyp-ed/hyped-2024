#pragma once

#include "logger.hpp"
#include "mqtt_topics.hpp"
#include "types.hpp"

#include <optional>
#include <queue>
#include <vector>

#include <mqtt/client.h>
#include <rapidjson/document.h>

namespace hyped::core {

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
  std::shared_ptr<rapidjson::Document> payload;
  bool operator<(const MqttMessage &other) const
  {
    if (header.priority == other.header.priority) {
      return header.timestamp > other.header.timestamp;
    }
    return header.priority < other.header.priority;
  }
  MqttMessage(MqttTopic topic, const Header &header, std::shared_ptr<rapidjson::Document> payload)
      : topic(topic),
        header(header),
        payload(std::move(payload)){};
};

class IMqtt {
 public:
  /**
   * @brief Sends a message to its topic
   *
   * @param message
   * @param qos
   * @return core::Result
   */
  virtual void publish(const MqttMessage &message, const MqttMessageQos qos) = 0;
  /**
   * @brief Subscribes to a topic
   *
   * @param topic
   * @return core::Result
   */
  virtual core::Result subscribe(const core::MqttTopic topic) = 0;
  /**
   * @brief Pulls next 100 MQTT messages into internal queue
   *
   * @returns kFailure if any message received is invalid, kSuccess otherwise
   */
  virtual core::Result consume() = 0;
  /**
   * @brief Returns the next message to be processed
   *
   * @returns next message to be processed, sorted by priority then delivery time. i.e. The highest
   * priority message is always selected, then the oldest in that priority is returned
   */
  virtual std::optional<MqttMessage> getMessage() = 0;
};

class Mqtt : public IMqtt {
 public:
  static std::optional<std::shared_ptr<Mqtt>> create(ILogger &logger,
                                                     const std::string &id,
                                                     const std::string &host,
                                                     const std::uint16_t port);
  Mqtt(ILogger &logger, std::unique_ptr<mqtt::client> client, mqtt::callback_ptr cb);
  ~Mqtt();
  void publish(const MqttMessage &message, const MqttMessageQos qos);
  core::Result subscribe(const core::MqttTopic topic);
  core::Result consume();
  std::optional<MqttMessage> getMessage();
  static constexpr std::uint8_t kKeepAliveInterval = 1;

 private:
  /**
   * @brief Creates an mqtt:message_ptr from an MqttMessage
   *
   * @param message
   * @return mqtt::message_ptr
   */
  mqtt::message_ptr messageToMessagePtr(const MqttMessage &message);
  /**
   * @brief Creates an MqttMessage from an mqtt:message_ptr
   *
   * @param message
   * @return MqttMessage if message contains valid header, nullopt otherwise
   */
  std::optional<MqttMessage> messagePtrToMessage(std::shared_ptr<const mqtt::message> message);

  ILogger &logger_;
  std::unique_ptr<mqtt::client> client_;
  std::priority_queue<MqttMessage> incoming_message_queue_;
  // callback_ptr needed to stop segfault per
  // https://github.com/eclipse/paho.mqtt.cpp/issues/141#issuecomment-375402234
  mqtt::callback_ptr cb;
};

class MqttCallback : public virtual mqtt::callback {
 public:
  MqttCallback(ILogger &logger);
  void connection_lost(const std::string &cause) override;

 private:
  ILogger &logger_;
};

}  // namespace hyped::core