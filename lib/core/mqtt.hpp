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
  MqttMessage(MqttTopic topic, Header &header, std::shared_ptr<rapidjson::Document> payload)
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
  virtual core::Result subscribe(const core::MqttTopic topic)                = 0;
  /**
   * @brief Pulls all pending MQTT messages into internal queue
   *
   * @returns kFailure if any message received is invalid, kSuccess otherwise
   */
  virtual core::Result consume() = 0;
  /**
   * @returns next message to be processed, sorted by priority then delivery time. i.e. The highest
   * priority message is always selected, then the oldest in that priority is returned
   *
   */
  virtual std::optional<MqttMessage> getMessage() = 0;
};

class Mqtt : public IMqtt {
 public:
  static std::optional<std::shared_ptr<Mqtt>> create(ILogger &logger,
                                                     const std::string &id,
                                                     const std::string &host,
                                                     const std::uint16_t port);
  Mqtt(ILogger &logger, std::unique_ptr<mqtt::client> client);
  ~Mqtt();
  void publish(const MqttMessage &message, const MqttMessageQos qos);
  core::Result subscribe(const core::MqttTopic topic);
  core::Result consume();
  std::optional<MqttMessage> getMessage();

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
  std::optional<MqttMessage> messagePtrToMessage(mqtt::const_message_ptr *message);

  ILogger &logger_;
  std::unique_ptr<mqtt::client> client_;
  std::priority_queue<MqttMessage> incoming_message_queue_;
  std::uint32_t messages_in_queue;
};

// class MqttCallback : public mqtt::callback {
//  public:
//   MqttCallback(ILogger &logger);
//   void connection_lost(const std::string &cause) override;
//   void delivery_complete(mqtt::delivery_token_ptr token) override;
//   void message_arrived(mqtt::const_message_ptr msg) override;

//  private:
//   ILogger &logger_;
//   std::shared_ptr<Mqtt> mqtt_;
// };

}  // namespace hyped::core